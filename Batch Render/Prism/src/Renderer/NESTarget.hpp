//
//  NESTarget.hpp
//  Batch Render
//
//  Created by William Bustamante on 10/10/26.
//
#pragma once
#include <Metal/Metal.hpp>
#include <array>

#include "Renderer.hpp"

namespace prism {

class Device;


class NESTarget {
public:
    
    NESTarget(Device& device, MTL::ResidencySet* residency);
    ~NESTarget();
    
    MTL::RenderCommandEncoder* begin_encoder(MTL::CommandBuffer* cmd_buffer, uint32_t index_frame);
    
    
    
private:
    //Variables
    MTL::RenderPassDescriptor* render_pass;
    std::array<MTL::Texture* , Renderer::MAX_FRAMES_IN_FLIGHT> render_pass_textures;
    std::array<MTL::Texture* , Renderer::MAX_FRAMES_IN_FLIGHT> depth_textures;
    
    // Functions
    void init(Device& device, MTL::ResidencySet* residency);
    
};

}
