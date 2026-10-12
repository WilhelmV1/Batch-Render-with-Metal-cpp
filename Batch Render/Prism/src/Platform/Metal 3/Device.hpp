//
//  Device.hpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once

#include <Metal/Metal.hpp>


namespace prism {

class Device {
public:
    
    Device();
    
    MTL::Device* get_device() { return device; };
    
    MTL::CommandQueue* create_new_command_queue() { return device->newCommandQueue(); }
    
    MTL::Buffer* create_buffer(size_t size);
    
    MTL::Library* get_defualt_library() { return device->newDefaultLibrary(); }
    
  

private:
    
    MTL::Device* device;
   
    
    void init();
    
    
    
};

}
