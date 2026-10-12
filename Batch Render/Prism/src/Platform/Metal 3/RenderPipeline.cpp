//
//  RenderPipeline.cpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//

#include "RenderPipeline.hpp"

#include "Device.hpp"
#include "Utils.hpp"

namespace prism {

static MTL::VertexFormat to_metal_format(VertexFormat format) {
    
    switch (format) {
        case VertexFormat::Float: return MTL::VertexFormatFloat;
        case VertexFormat::Float2: return MTL::VertexFormatFloat2;
        case VertexFormat::Float3: return MTL::VertexFormatFloat3;
        case VertexFormat::Float4: return MTL::VertexFormatFloat4;
        case VertexFormat::Int: return MTL::VertexFormatInt;
        case VertexFormat::Int2: return MTL::VertexFormatInt2;
        case VertexFormat::Int3: return MTL::VertexFormatInt3;
        case VertexFormat::Int4: return MTL::VertexFormatInt4;
        case VertexFormat::Uint: return MTL::VertexFormatUInt;
        case VertexFormat::Uint2: return MTL::VertexFormatUInt2;
        case VertexFormat::Uint3: return MTL::VertexFormatUInt3;
        case VertexFormat::Uint4: return MTL::VertexFormatUInt4;
        default: return MTL::VertexFormatInvalid;
    }
};

RenderPipeline::RenderPipeline(Device& device, const RenderPipelineCreateInfo& create_info) {
    
    MTL::Library* library = device.get_defualt_library();
    MTL::Function* vertex_fn = library->newFunction(to_ns_string(create_info.vertex_name));
    MTL::Function* fragment_fn = library->newFunction(to_ns_string(create_info.fragment_name));
    
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    if (!create_info.pipeline_name.empty()) descriptor->setLabel(to_ns_string(create_info.pipeline_name));
    descriptor->setFragmentFunction(fragment_fn);
    descriptor->setVertexFunction(vertex_fn);
    descriptor->colorAttachments()->object(0)->setPixelFormat(create_info.color_format);
    descriptor->setDepthAttachmentPixelFormat(create_info.depth_format);
    
    MTL::VertexDescriptor* vertex_descriptor = MTL::VertexDescriptor::alloc()->init();
    for (const auto& attrib: create_info.attributes) {
        MTL::VertexAttributeDescriptor* attributes = vertex_descriptor->attributes()->object(attrib.location);
        attributes->setFormat(to_metal_format(attrib.format));
        attributes->setOffset(attrib.offset);
        attributes->setBufferIndex(attrib.buffer_index);
    }
    vertex_descriptor->layouts()->object(0)->setStride(create_info.layout.stride);
    
    descriptor->setVertexDescriptor(vertex_descriptor);
    
    if (create_info.enable_blending) {
        auto* color_attach = descriptor->colorAttachments()->object(0);
        color_attach->setBlendingEnabled(true);
        color_attach->setRgbBlendOperation(MTL::BlendOperationAdd);
        color_attach->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
        color_attach->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
        color_attach->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
        color_attach->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    }
    
    NS::Error* err = nullptr;
    pipeline = device.get_device()->newRenderPipelineState(descriptor, &err);
    if (!pipeline) {
        std::string msg = err ? err->localizedFailureReason()->utf8String() : "Unknown";
        if (err) err->release();
        throw std::runtime_error("Failed to create Render Pipeline" + msg);
    }
    vertex_descriptor->release();
    fragment_fn->release();
    vertex_fn->release();
    library->release();
    descriptor->release();
}

void RenderPipeline::bind(MTL::RenderCommandEncoder *encoder) {
    encoder->setRenderPipelineState(pipeline);
}

}

