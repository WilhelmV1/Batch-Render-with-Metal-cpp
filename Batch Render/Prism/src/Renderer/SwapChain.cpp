//
//  SwapChain.cpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//

#include "SwapChain.hpp"
#include <Metal/Metal.hpp>

namespace prism {

struct SwapChainData {
    
    MTL::RenderPassDescriptor* render_pass;
    
};

static SwapChainData s_data;

void SwapChain::init() {
    
    s_data.render_pass = MTL::RenderPassDescriptor::alloc()->init();
    auto attachments = s_data.render_pass->colorAttachments();
    
    attachments->object(0)->setLoadAction(MTL::LoadActionClear);
    attachments->object(0)->setStoreAction(MTL::StoreActionStore);
    attachments->object(0)->setClearColor(MTL::ClearColor(0, 0, 0, 1.0f));
}

void SwapChain::shutdown() {
    if (s_data.render_pass) s_data.render_pass->release();
}

MTL::RenderCommandEncoder* SwapChain::begin_swap_chain(MTL::CommandBuffer *cmd_buffer, MTL::Texture *texture) {
    s_data.render_pass->colorAttachments()->object(0)->setTexture(texture);

    return cmd_buffer->renderCommandEncoder(s_data.render_pass);
    
}

void SwapChain::end_swap_chain(MTL::RenderCommandEncoder *encoder) {
    encoder->endEncoding();
}

}
