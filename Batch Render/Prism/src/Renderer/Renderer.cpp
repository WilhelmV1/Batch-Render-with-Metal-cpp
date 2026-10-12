//
//  Renderer.cpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//

#include "Renderer.hpp"

#include <SDL3/SDL_log.h>

#include "Device.hpp"
#include "FrameContext.hpp"
#include "Layer.hpp"


namespace prism {

struct RendererData {
    //Metal Data
    MTL::CommandQueue* command_queue = nullptr;
    MTL::ResidencySet* residency_set = nullptr;
    MTL::SharedEvent* event = nullptr;
    Layer* layer = nullptr;
    
    
    // Event Data
    std::uint64_t frame_number = 0;
    std::uint32_t frame_index = 0;
};

static RendererData s_data;


void Renderer::init(Device &device, Layer& layer) {
    
    s_data.command_queue = device.create_new_command_queue();
    
    if (!s_data.command_queue) throw std::runtime_error("Failed to create Command Queue ");
    
    MTL::ResidencySetDescriptor* descriptor = MTL::ResidencySetDescriptor::alloc()->init(); NS::Error* error = nullptr;
    
    descriptor->setInitialCapacity(16);
    
    s_data.residency_set = device.get_device()->newResidencySet(descriptor, &error);
    
    descriptor->release();
    
    if (!s_data.residency_set) {
        std::string msg = error ? error->localizedFailureReason()->utf8String() : "unknow error";
        if (error) error->release();
        throw std::runtime_error("Failed to create Residency Set " + msg);
    }
    
    s_data.event = device.get_device()->newSharedEvent();
    
    if (!s_data.event) {
        throw std::runtime_error("Failed to create Metal Shared Event");
    }
    s_data.event->setSignaledValue(s_data.frame_number);
    
    s_data.layer = &layer;
}

void Renderer::shutdown() {
    if (s_data.event) { s_data.event->release(); s_data.event = nullptr; }
    if (s_data.residency_set) { s_data.residency_set->release(); s_data.residency_set = nullptr; }
    if (s_data.command_queue) {s_data.command_queue->release(); s_data.command_queue = nullptr; }
    if (s_data.layer) s_data.layer = nullptr;
    
}

FrameContext Renderer::begin_frame() {
    FrameContext ctx;
    ctx.drawable = s_data.layer->get_next_drawable();
    ctx.command_buffer = s_data.command_queue->commandBuffer();
    ctx.residency_set = s_data.residency_set;
    ctx.frame_index = s_data.frame_index;
    
    return ctx;
}

void Renderer::end_frame(FrameContext &ctx) {
    ctx.command_buffer->useResidencySet(ctx.residency_set);
    ctx.command_buffer->encodeSignalEvent(s_data.event, s_data.frame_number);
    ctx.command_buffer->presentDrawable(ctx.drawable);
    ctx.command_buffer->commit();
    
    next_event_frame();
}

void Renderer::next_event_frame() {
    s_data.frame_number++;
    
    s_data.frame_index = s_data.frame_number % MAX_FRAMES_IN_FLIGHT;
    
    if (s_data.frame_number > MAX_FRAMES_IN_FLIGHT) {
        const std::uint64_t milisecons = 100;
        
        bool beforeTimeout = s_data.event->waitUntilSignaledValue(s_data.frame_number - MAX_FRAMES_IN_FLIGHT, milisecons);
        
        if(!beforeTimeout){
            SDL_Log("No signal from frame %llu to shared event after %llums", s_data.frame_number -  MAX_FRAMES_IN_FLIGHT, milisecons);
        }
    }
}

MTL::ResidencySet* Renderer::get_residency() {
    if (!s_data.residency_set) return nullptr;
    return s_data.residency_set;
}

}
