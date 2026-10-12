//
//  Application.cpp
//  Super Mario Bros 3
//
//  Created by William Bustamante on 05/10/26.
//

#include "Application.hpp"

#include "Renderer.hpp"
#include "FrameContext.hpp"
#include "BatchRenderer.hpp"

#include "NESTarget.hpp"
#include "SwapChain.hpp"

namespace prism {


App::App() {
    
    Renderer::init(device, layer);
    BatchRender::init(device, Renderer::get_residency());
    SwapChain::init();
}

App::~App(){
    Renderer::shutdown();
    BatchRender::shutdown();
    SwapChain::shutdown();
}

void App::run() {
    
    std::unique_ptr<NESTarget> NES_target = std::make_unique<NESTarget>(device, Renderer::get_residency());
    auto last_time = SDL_GetTicksNS();
    
    while (window.is_open()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                window.close();
            }
        }
        
        auto current_time = SDL_GetTicksNS();
        
        float delta_time = static_cast<float>(current_time - last_time) / 1000000000.0f;
        
        last_time = current_time;
        
        NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
        
        auto ctx = Renderer::begin_frame();
        ctx.delta_time = delta_time;
        
        MTL::RenderCommandEncoder* encoder = NES_target->begin_encoder(ctx.command_buffer, ctx.frame_index);

        
        encoder->endEncoding();
    
        encoder = SwapChain::begin_swap_chain(ctx.command_buffer, ctx.drawable->texture());
        
        ctx.nes_encoder = encoder;
        
        BatchRender::begin_batch(ctx);
        
        BatchRender::draw_multicolor_quad(0.0f, 0.0f, 0.0f, 32.0f, 32.0f, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f,0.0f,1.0f});
        
        BatchRender::present();
        
        SwapChain::end_swap_chain(encoder);
        
        Renderer::end_frame(ctx);
        
        pool->release();
        
    }
}

}
