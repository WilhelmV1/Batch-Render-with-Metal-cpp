//
//  Buffer.hpp
//  Batch Render
//
//  Created by William Bustamante on 08/10/26.
//
#pragma once

#include <cstdio>

#include <Metal/Metal.hpp>

namespace prism {

class Device;

class Buffer {
public:
    
    virtual ~Buffer() {
        if (buffer) buffer->release();
    }
    
    void set_label(std::string_view label);
    
    void set_data(const void* data);
    
    void make_resident(MTL::ResidencySet& residency) const;
    
    template<class T>
    T* map() const {
        static_assert(!std::is_pointer_v<T>, "Must be a non pointer type");
        return reinterpret_cast<T*>(buffer->contents());
    }
    
protected:
    
    Buffer(Device& device, size_t size);
    
    MTL::Buffer* buffer = nullptr;
    size_t buffer_size = 0;
    
};

class VertexBuffer : public Buffer {
public:
    
    VertexBuffer(Device& device, size_t size);
    
    void bind(MTL::RenderCommandEncoder* encoder, uint8_t index, MTL::RenderStages stage) const;
    
};

class IndexBuffer : public Buffer {
public:
    
    IndexBuffer(Device& device, size_t elements_size);
    
    MTL::Buffer* get_index_buffer() { return buffer; }
};

}
