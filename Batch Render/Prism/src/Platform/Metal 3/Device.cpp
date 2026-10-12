//
//  Device.cpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//

#include "Device.hpp"


namespace prism {

Device::Device() {
    init();
}

void Device::init() {
    
    device = MTL::CreateSystemDefaultDevice();
    
    assert(device && "Failed to create device");
}

MTL::Buffer* Device::create_buffer(size_t size) {
    return device->newBuffer(size, MTL::ResourceStorageModeShared);
}

}
