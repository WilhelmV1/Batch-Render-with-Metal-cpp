//
//  Layer.hpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <SDL3/SDL_metal.h>

namespace prism {

class Device;
class Window;

class Layer {
public:
    
    Layer(Window& window, Device& device);
    ~Layer();
    
    CA::MetalLayer* get_layer() const { return layer; }
    CA::MetalDrawable* get_next_drawable() { return layer->nextDrawable(); }
    MTL::PixelFormat const get_pixel_format() { return layer->pixelFormat(); }
    CGSize const get_window_size(){ return layer->drawableSize(); }
    MTL::ResidencySet* const get_layer_residency() { return layer->residencySet(); }
    
private:
    
    Window& window;
    Device& device;
    
    SDL_MetalView view;
    CA::MetalLayer* layer;
    
    void init_metal_layer();
};

}
