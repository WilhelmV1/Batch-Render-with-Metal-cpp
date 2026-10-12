//
//  BatchRenderer.cpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//

#include "BatchRenderer.hpp"

#include <array>

#include "ShaderTypes.hpp"
#include "MetalBuffer.hpp"
#include "Device.hpp"
#include "FrameContext.hpp"
#include "MetalTexture.hpp"
#include "Palette.hpp"
#include "RenderPipeline.hpp"
#include "Camera2D.hpp"

namespace prism {

enum class BatchStates{
    IDLE,
    RECORDING
};

struct BatchRenderData {
    
    static constexpr std::uint8_t MAX_FRAMES_IN_FLIGHT = 3;
    static const std::uint32_t max_quads = 1000;
    static const std::uint32_t max_vertices = max_quads * 4;
    static const std::uint32_t max_indices = max_quads * 6;
    
    std::array<std::unique_ptr<VertexBuffer>, MAX_FRAMES_IN_FLIGHT> vertex_buffers;
    std::unique_ptr<IndexBuffer> index_buffer;
    
    uint32_t sprite_index_count = 0;
    uint32_t sprite_vertex_count = 0;
    
    Vertex* buffer = nullptr;
    Vertex* buffer_start = nullptr;
    VertexBuffer* current_buffer = nullptr;
    
    std::array<std::unique_ptr<VertexBuffer>, MAX_FRAMES_IN_FLIGHT> quads_vertex_buffers;
    uint32_t quad_index_count = 0;
    uint32_t quad_vertex_count = 0;
    
    QuadVertex* quad_buffer = nullptr;
    QuadVertex* quad_buffer_start = nullptr;
    VertexBuffer* quad_current_buffer = nullptr;
    
    std::unique_ptr<VertexBuffer> nes_colors;
    std::unique_ptr<VertexBuffer> nes_palettes;
    std::unique_ptr<VertexBuffer> ubo;
    std::unique_ptr<ArrayTexture2D> textures;
    
    Camera2D camera{};
    
    MTL::RenderCommandEncoder* nes_encoder = nullptr;
    uint32_t frame_index = 0;
    BatchStates states = BatchStates::IDLE;
    
    std::unique_ptr<RenderPipeline> sprite_pipeline = nullptr;
    std::unique_ptr<RenderPipeline> quad_pipeline = nullptr;
};

static BatchRenderData s_data;


void BatchRender::init(Device& device, MTL::ResidencySet* residency) {
    
    // Init Vertex Buffers
    size_t vertex_size = s_data.max_vertices * sizeof(Vertex);
    size_t index_size = s_data.max_indices * sizeof(std::uint32_t);
    
    for (auto& vertex_buffer : s_data.vertex_buffers) {
        vertex_buffer = std::make_unique<VertexBuffer>(device, vertex_size);
        vertex_buffer->set_label("Vertex Sprite Buffer");
    }
    
    vertex_size = s_data.max_vertices * sizeof(QuadVertex);
    
    for (auto& vertex_buffer : s_data.quads_vertex_buffers) {
        vertex_buffer = std::make_unique<VertexBuffer>(device, vertex_size);
        vertex_buffer->set_label("Vertex Quad Buffer");
    }
    
    // Init Resource Buffers
    
    std::vector<NES::Palette> palettes = {
        {NES::COLOR_01, NES::COLOR_30, NES::COLOR_27, NES::COLOR_0D}, // 0
        {NES::COLOR_30, NES::COLOR_06, NES::COLOR_01, NES::COLOR_0D}, // 1
        {NES::COLOR_0D, NES::COLOR_29, NES::COLOR_19, NES::COLOR_1D}, // 2
        {NES::COLOR_0D, NES::COLOR_30, NES::COLOR_06, NES::COLOR_1D}, // 3
        {NES::COLOR_0D, NES::COLOR_30, NES::COLOR_1D, NES::COLOR_01}, // 4
        {NES::COLOR_0D, NES::COLOR_30, NES::COLOR_1D, NES::COLOR_01}, // 5
        {NES::COLOR_19, NES::COLOR_29, NES::COLOR_19, NES::COLOR_09}, // 6
        {NES::COLOR_20, NES::COLOR_26, NES::COLOR_07, NES::COLOR_0D}, // 7
        {NES::COLOR_10, NES::COLOR_19, NES::COLOR_11, NES::COLOR_0D}, // 8
        {NES::COLOR_30, NES::COLOR_27, NES::COLOR_30, NES::COLOR_1D}, // 9
        {NES::COLOR_0D, NES::COLOR_1D, NES::COLOR_17, NES::COLOR_1D}, // 10
        {NES::COLOR_0D, NES::COLOR_1D, NES::COLOR_1D, NES::COLOR_1D}, // 11
        {NES::COLOR_1D, NES::COLOR_1D, NES::COLOR_1D, NES::COLOR_0D}, // 12
        {NES::COLOR_0D, NES::COLOR_30, NES::COLOR_25, NES::COLOR_1D}, // 13
        {NES::COLOR_0D, NES::COLOR_30, NES::COLOR_00, NES::COLOR_1D}, // 14
        {NES::COLOR_0D, NES::COLOR_27, NES::COLOR_17, NES::COLOR_1D}, // 15
    };
    
    size_t buffer_size = sizeof(NES::Palette) * palettes.size();
    s_data.nes_palettes = std::make_unique<VertexBuffer>(device, buffer_size);
    s_data.nes_palettes->set_data(palettes.data());
    s_data.nes_palettes->set_label("NES Palettes");
    
    buffer_size = sizeof(simd::float4) * (int)NES::COLOR_COUNT;
    s_data.nes_colors = std::make_unique<VertexBuffer>(device, buffer_size);
    s_data.nes_colors->set_data(NES::colors);
    s_data.nes_colors->set_label("NES Colors");
    
    UniformBuffer ubo{};
    s_data.camera.set_orthographic_projection(0, 256, 0, 224, 10, -10);
    ubo.projection = s_data.camera.get_ortho_matrix();
    
    buffer_size = sizeof(UniformBuffer);
    s_data.ubo = std::make_unique<VertexBuffer>(device, buffer_size);
    s_data.ubo->set_data(&ubo);
    
    uint32_t offset = 0;
    
    std::vector<uint32_t> indices(s_data.max_indices);
    
    for (uint32_t i = 0; i < s_data.max_indices; i += 6) {
        
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        
        offset += 4;
    }
    
    s_data.index_buffer = std::make_unique<IndexBuffer>(device, index_size);
    s_data.index_buffer->set_data(indices.data());
    
    s_data.textures = std::make_unique<ArrayTexture2D>();
    
    std::vector<std::string_view> paths = {
        "SMB2 Toad.png",
        "SMB2 Tiles.png",
        "SMB2 Items.png",
        "SMB2 Enemy.png",
        "SMB2 Animated Tiles.png"
    };
    
    //s_data.textures->load_texture_from_files(paths, 256, 256, device, MTL::PixelFormatR8Uint, MTL::TextureUsageShaderRead);
    
    s_data.index_buffer->make_resident(*residency);
    s_data.nes_colors->make_resident(*residency);
    s_data.nes_palettes->make_resident(*residency);
   // s_data.textures->make_resident(*residency);
    
    for (auto& v : s_data.vertex_buffers) {
        v->make_resident(*residency);
    }
    
    RenderPipelineCreateInfo sprite_create_info{}, quad_create_info{};
    
    sprite_create_info.attributes = {
        VertexAttributes{.format = VertexFormat::Float3, .offset = offsetof(Vertex, position), .buffer_index = 0, .location = 0},
        VertexAttributes{.format = VertexFormat::Float2, .offset = offsetof(Vertex, uv), .buffer_index = 0, .location = 1},
        VertexAttributes{.format = VertexFormat::Uint, .offset = offsetof(Vertex, palette_index), .buffer_index = 0, .location = 2},
        VertexAttributes{.format = VertexFormat::Uint, .offset = offsetof(Vertex, texture_index), .buffer_index = 0, .location = 3}
    };
    
    sprite_create_info.layout.stride = sizeof(Vertex);
    sprite_create_info.vertex_name = "vertex_shader";
    sprite_create_info.fragment_name = "fragment_shader";
    sprite_create_info.pipeline_name = "Sprite Render Pipeline";
    sprite_create_info.color_format = MTL::PixelFormatBGRA8Unorm;
    sprite_create_info.depth_format = MTL::PixelFormatInvalid;
    sprite_create_info.enable_blending = true;
   
    
    quad_create_info.attributes = {
        VertexAttributes{.format = VertexFormat::Float3, .offset = offsetof(QuadVertex, position), .buffer_index = 0, .location = 0},
        VertexAttributes{.format = VertexFormat::Float3, .offset = offsetof(QuadVertex, color), .buffer_index = 0, .location = 1}
    };
    
    quad_create_info.layout.stride = sizeof(QuadVertex);
    quad_create_info.vertex_name = "quad_vertex_shader";
    quad_create_info.fragment_name = "quad_fragment_shader";
    quad_create_info.pipeline_name = "Quad Render Pipeline";
    quad_create_info.color_format = MTL::PixelFormatBGRA8Unorm;
    quad_create_info.depth_format = MTL::PixelFormatInvalid;
   
    
    s_data.sprite_pipeline = std::make_unique<RenderPipeline>(device, sprite_create_info);
    s_data.quad_pipeline = std::make_unique<RenderPipeline>(device, quad_create_info);
    
    
}

void BatchRender::shutdown() {
    s_data.buffer = nullptr;
    s_data.buffer_start = nullptr;
    s_data.current_buffer = nullptr;
    
    s_data.quad_buffer = nullptr;
    s_data.quad_buffer_start = nullptr;
    s_data.quad_current_buffer = nullptr;
    
    s_data.nes_encoder = nullptr;
    s_data.sprite_index_count = 0;
    s_data.sprite_vertex_count = 0;
    s_data.quad_index_count = 0;
    s_data.quad_vertex_count = 0;
    s_data.states = BatchStates::IDLE;
    s_data.frame_index = 0;
}

void BatchRender::draw_sprite(float x, float y, float z, float w, float h, simd::float2 sprite_pos, simd::float2 sprite_size, simd::uint1 palette_index, simd::uint1 texture_index) {
    
    assert(s_data.states == BatchStates::RECORDING && "Error, you can´t draw, pls use Begin Batch before");
    
    if (s_data.sprite_index_count + 6 > s_data.max_indices || s_data.sprite_vertex_count + 4 > s_data.max_vertices) {
        flush();
        reset();
    }
    
    simd::float3 position = simd_make_float3(x, y, z);
    simd::float2 size = simd::make_float2(w, h);
    
    simd::float2 uv_min = sprite_pos;
    simd::float2 uv_max = (sprite_pos + sprite_size);
    
    push_vertex(position, uv_min, palette_index, texture_index);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y, position.z), simd_make_float2(uv_max.x, uv_min.y), palette_index, texture_index);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y + size.y, position.z), uv_max, palette_index, texture_index);
    
    push_vertex(simd_make_float3(position.x, position.y + size.y, position.z), simd_make_float2(uv_min.x, uv_max.y), palette_index, texture_index);
    
    s_data.sprite_vertex_count += 4;
    s_data.sprite_index_count += 6;
}

void BatchRender::draw_quad(float x, float y, float z, float w, float h, simd::float3 color) {
    assert(s_data.states == BatchStates::RECORDING && "Error, you can´t draw, pls use Begin Batch before");
    
    if (s_data.quad_index_count + 6 > s_data.max_indices || s_data.quad_vertex_count + 4 > s_data.max_vertices) {
        flush();
        reset();
    }
    
    simd::float3 position = simd_make_float3(x, y, z);
    simd::float2 size = simd::make_float2(w, h);
    
    push_vertex(position, color);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y, position.z), color);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y + size.y, position.z), color);
    
    push_vertex(simd_make_float3(position.x, position.y + size.y, position.z), color);
    
    s_data.quad_vertex_count += 4;
    s_data.quad_index_count += 6;
}

void BatchRender::draw_multicolor_quad(float x, float y, float z, float w, float h, simd::float3 color1, simd::float3 color2, simd::float3 color3, simd::float3 color4) {
    
    assert(s_data.states == BatchStates::RECORDING && "Error, you can´t draw, pls use Begin Batch before");
    
    if (s_data.quad_index_count + 6 > s_data.max_indices || s_data.quad_vertex_count + 4 > s_data.max_vertices) {
        flush();
        reset();
    }
    
    simd::float3 position = simd_make_float3(x, y, z);
    simd::float2 size = simd::make_float2(w, h);
    
    push_vertex(position, color1);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y, position.z), color2);
    
    push_vertex(simd_make_float3(position.x + size.x, position.y + size.y, position.z), color3);
    
    push_vertex(simd_make_float3(position.x, position.y + size.y, position.z), color4);
    
    s_data.quad_vertex_count += 4;
    s_data.quad_index_count += 6;
}

void BatchRender::push_vertex(simd::float3 position, simd::float2 uv, simd::uint1 palette_index, simd::uint1 texture_index) {
    assert(s_data.sprite_vertex_count < s_data.max_vertices);
    s_data.buffer->position = position;
    s_data.buffer->uv =  uv;
    s_data.buffer->palette_index = palette_index;
    s_data.buffer->texture_index = texture_index;
    s_data.buffer++;
}

void BatchRender::push_vertex(simd::float3 position, simd::float3 color) {
    assert(s_data.quad_vertex_count < s_data.max_vertices);
    s_data.quad_buffer->position = position;
    s_data.quad_buffer->color = color;
    
    s_data.quad_buffer++;
}

void BatchRender::begin_batch(const FrameContext &context) {
    
    assert(s_data.states == BatchStates::IDLE && "You can´t use begin bacth during the recording");
    
    reset();
    
    s_data.states = BatchStates::RECORDING;
    s_data.frame_index = context.frame_index;
    s_data.nes_encoder = context.nes_encoder;
    
    s_data.current_buffer = s_data.vertex_buffers[s_data.frame_index].get();
    s_data.buffer_start = s_data.current_buffer->map<Vertex>();
    s_data.buffer = s_data.buffer_start;
    
    s_data.quad_current_buffer = s_data.quads_vertex_buffers[s_data.frame_index].get();
    s_data.quad_buffer_start = s_data.quad_current_buffer->map<QuadVertex>();
    s_data.quad_buffer = s_data.quad_buffer_start;
    
    UniformBuffer *ubo = s_data.ubo->map<UniformBuffer>();
    ubo->dt += context.delta_time;
    
}

void BatchRender::present() {
    
    assert(s_data.states == BatchStates::RECORDING && "Error, you can´t present, please use begin batch before");
    
    flush();
    reset();
    
    s_data.states = BatchStates::IDLE;
    s_data.current_buffer = nullptr;
}

void BatchRender::flush() {
    if (!s_data.nes_encoder) return;
    
    s_data.ubo->bind(s_data.nes_encoder, 1, MTL::RenderStageVertex | MTL::RenderStageFragment);
    //s_data.nes_encoder->setFragmentTexture(s_data.textures->get_texture(), 0);
    s_data.nes_palettes->bind(s_data.nes_encoder, 2, MTL::RenderStageFragment);
    s_data.nes_colors->bind(s_data.nes_encoder, 3, MTL::RenderStageFragment);
    
    if (s_data.sprite_index_count != 0) {
        s_data.sprite_pipeline->bind(s_data.nes_encoder);
        s_data.current_buffer->bind(s_data.nes_encoder, 0, MTL::RenderStageVertex);
        s_data.nes_encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, s_data.sprite_index_count, MTL::IndexTypeUInt32, s_data.index_buffer->get_index_buffer(), 0, 1);
    }
    
    if (s_data.quad_index_count != 0) {
        s_data.quad_pipeline->bind(s_data.nes_encoder);
        s_data.quad_current_buffer->bind(s_data.nes_encoder, 0, MTL::RenderStageVertex);
        s_data.nes_encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, s_data.quad_index_count, MTL::IndexTypeUInt32, s_data.index_buffer->get_index_buffer(), 0, 1);
    }
}

void BatchRender::reset() {
    s_data.sprite_index_count = 0;
    s_data.sprite_vertex_count = 0;
    s_data.quad_index_count = 0;
    s_data.quad_vertex_count = 0;
    s_data.buffer = nullptr;
    s_data.quad_buffer = nullptr;
}

}
