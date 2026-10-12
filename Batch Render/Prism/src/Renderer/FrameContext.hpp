//
//  FrameContext.hpp
//  Batch Render
//
//  Created by William Bustamante on 06/10/26.
//
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace prism {

struct FrameContext {
    CA::MetalDrawable* drawable = nullptr;
    MTL::CommandBuffer* command_buffer = nullptr;
    MTL::ResidencySet* residency_set = nullptr;
    std::uint32_t frame_index = 0;
    
    MTL::RenderCommandEncoder* nes_encoder = nullptr;
    float delta_time;
};


}

