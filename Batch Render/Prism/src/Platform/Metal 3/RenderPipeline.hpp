//
//  RenderPipeline.hpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//
#pragma once
#include <vector>
#include <Metal/Metal.hpp>

namespace prism {

class Device;

enum class VertexFormat {
    Float, Float2, Float3, Float4,
    Int, Int2, Int3, Int4,
    Uint, Uint2, Uint3, Uint4
};

struct VertexAttributes {
    VertexFormat format;
    uint32_t offset;
    uint32_t buffer_index;
    uint32_t location;
    
};

struct VertexLayout {
    uint32_t stride;
};

struct RenderPipelineCreateInfo {
    std::string vertex_name;
    std::string fragment_name;
    std::string pipeline_name;
    
    std::vector<VertexAttributes> attributes;
    VertexLayout layout{};
    
    MTL::PixelFormat color_format;
    MTL::PixelFormat depth_format;
    
    bool enable_blending = false;
};

class RenderPipeline {
public:
    
    RenderPipeline(Device& device, const RenderPipelineCreateInfo& create_info);
    
    void bind(MTL::RenderCommandEncoder* encoder);
    
private:
    
    MTL::RenderPipelineState* pipeline = nullptr;
};

}
