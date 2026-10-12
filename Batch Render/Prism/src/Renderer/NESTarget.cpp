//
//  NESTarget.cpp
//  Batch Render
//
//  Created by William Bustamante on 10/10/26.
//

#include "NESTarget.hpp"

#include "Layer.hpp"
#include "Device.hpp"

namespace prism {

NESTarget::NESTarget(Device& device, MTL::ResidencySet* residency) {
    init(device, residency);
}

NESTarget::~NESTarget() {
    if (render_pass) render_pass->release();
}

void NESTarget::init(Device& device, MTL::ResidencySet* residency) {
    
    MTL::TextureDescriptor* tex_descriptor = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatBGRA8Unorm, 256, 224, false);
    tex_descriptor->setStorageMode(MTL::StorageModePrivate);
    tex_descriptor->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
    tex_descriptor->setMipmapLevelCount(1);
    
    for (auto& texture : render_pass_textures) {
        texture = device.get_device()->newTexture(tex_descriptor);
        residency->addAllocation(texture);
    }
    
    tex_descriptor = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatDepth16Unorm, 256, 224, false);
    
    tex_descriptor->setStorageMode(MTL::StorageModePrivate);
    tex_descriptor->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
    tex_descriptor->setMipmapLevelCount(1);
    
    
    for (auto& depth_texture : depth_textures) {
        depth_texture = device.get_device()->newTexture(tex_descriptor);
        residency->addAllocation(depth_texture);
    }
    
    render_pass = MTL::RenderPassDescriptor::alloc()->init();
    
    auto attachments = render_pass->colorAttachments();
    
    attachments->object(0)->setLoadAction(MTL::LoadActionClear);
    attachments->object(0)->setStoreAction(MTL::StoreActionStore);
    attachments->object(0)->setClearColor(MTL::ClearColor(0, 0, 0, 1.0f));
    
    auto depth_attachment = render_pass->depthAttachment();
    depth_attachment->setLoadAction(MTL::LoadActionClear);
    depth_attachment->setStoreAction(MTL::StoreActionDontCare);
    depth_attachment->setClearDepth(1.0f);
   
}

MTL::RenderCommandEncoder* NESTarget::begin_encoder(MTL::CommandBuffer *cmd_buffer, uint32_t index_frame) {
    render_pass->colorAttachments()->object(0)->setTexture(render_pass_textures[index_frame]);
    render_pass->depthAttachment()->setTexture(depth_textures[index_frame]);
    
    return cmd_buffer->renderCommandEncoder(render_pass);
}



}
