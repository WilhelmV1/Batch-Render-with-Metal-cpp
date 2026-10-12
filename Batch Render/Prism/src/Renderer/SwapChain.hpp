//
//  SwapChain.hpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//
#pragma once

namespace MTL {

class Texture;
class RenderCommandEncoder;
class CommandBuffer;
}

namespace prism {

class SwapChain {
public:
    
    static void init();
    static void shutdown();
    
    static MTL::RenderCommandEncoder* begin_swap_chain(MTL::CommandBuffer *command_buffer, MTL::Texture* texture);
    
    static void end_swap_chain(MTL::RenderCommandEncoder* encoder);


};

}
