//
//  Renderer.hpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once
#include <cstdint>
#include <Metal/Metal.hpp>

namespace prism {

class Device;
class Layer;

struct FrameContext;

class Renderer {
public:
    
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 3;
    
    static void init(Device& device, Layer& layer);
    static void shutdown();
    
    static void next_event_frame();
    
    static MTL::ResidencySet* get_residency();
    
    static FrameContext begin_frame();
    
    static void end_frame(FrameContext& ctx);
    
private:
    
};

}
