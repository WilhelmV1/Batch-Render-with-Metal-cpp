//
//  Layer.cpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//

#include "Layer.hpp"

#include "Window.hpp"
#include "Device.hpp"

namespace prism {

Layer::Layer(Window& w, Device& d) : window(w), device(d) {
    init_metal_layer();
}

Layer::~Layer() {
    if (view) SDL_Metal_DestroyView(view);
    if (layer) layer->release();
}

void Layer::init_metal_layer() {
    
    view = SDL_Metal_CreateView(window.get_window());
    
    assert(view && "Failed to crerate View");
    
    layer = (CA::MetalLayer*)SDL_Metal_GetLayer(view);
    
    assert(layer && "Failed to acquire Metal Layer");
    
    layer->setMaximumDrawableCount(2);
    layer->setDevice(device.get_device());
    layer->setFramebufferOnly(false);
    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    layer->setDisplaySyncEnabled(true);
    layer->setAllowsNextDrawableTimeout(false);
}

}
