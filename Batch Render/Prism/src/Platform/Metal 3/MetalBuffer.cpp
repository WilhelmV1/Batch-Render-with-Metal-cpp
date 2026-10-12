//
//  Buffer.cpp
//  Batch Render
//
//  Created by William Bustamante on 08/10/26.
//

#include "MetalBuffer.hpp"

#include <stdexcept>

#include "Device.hpp"
#include "Utils.hpp"

namespace prism {

Buffer::Buffer(Device& device, size_t size) : buffer_size(size){
    buffer = device.create_buffer(buffer_size);
    if (!buffer) throw std::runtime_error("Failed to create buffer");
}

void Buffer::set_label(std::string_view label) {
    assert(buffer && "Buffer is null");
    buffer->setLabel(to_ns_string(label));
}

void Buffer::set_data(const void *data) {
    assert(buffer && data && "Buffer or data is null");
    std::memcpy(buffer->contents(), data, buffer_size);
}

void Buffer::make_resident(MTL::ResidencySet &residency) const {
    if (!buffer) return;
    residency.addAllocation(buffer);
}

VertexBuffer::VertexBuffer(Device& device, size_t size) : Buffer(device, size) {}

void VertexBuffer::bind(MTL::RenderCommandEncoder *encoder, uint8_t index, MTL::RenderStages stage) const {
    if (stage & MTL::RenderStageVertex) {
        encoder->setVertexBuffer(buffer, 0, index);
    }
    if (stage & MTL::RenderStageFragment) {
        encoder->setFragmentBuffer(buffer, 0, index);
    }
    
}

IndexBuffer::IndexBuffer(Device& device, size_t size) : Buffer(device, size) {}

}
