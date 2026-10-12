//
//  MetalTexture.hpp
//  Batch Render
//
//  Created by William Bustamante on 09/10/26.
//
#pragma once

//
//  mtl_texture.hpp
//  Super Mario Bros 2
//
//  Created by William Bustamante on 28/03/26.
//
#pragma once

#include <simd/simd.h>

#include "Device.hpp"

namespace prism {

using pixels = unsigned char;

    class Texture{
    protected:
        
        MTL::Texture* texture = nullptr;
        
        pixels* load_texture(std::string_view filename);
        
    public:
        std::uint32_t size;
        int height = 0 , width = 0;
        std::uint32_t mipMapCount;
        
        //MTL::Texture* getTexture() const { return texture; }
       
        MTL::ResourceID get_id() const {return texture->gpuResourceID(); }
        MTL::Texture* get_texture() { return texture; }
        void make_resident(MTL::ResidencySet& set) const;
        simd::float2 get_texture_size() const { return simd_make_float2(texture->width(), texture->height()); };
        
        void destroy();
    
    };

    class Texture2D : public Texture {
    public:
        void load_texture_from_file(std::string_view filename, Device& device, MTL::PixelFormat format, MTL::TextureUsage usage, bool mipmaped = false, MTL::StorageMode mode = MTL::StorageModeShared);
        
        void create_depth_texture(Device& device, MTL::PixelFormat format, simd::float2 size);
    

    };

    class ArrayTexture2D : public Texture{
    public:
        
        void load_texture_from_files(std::vector<std::string_view> filenames ,int width,int height,Device& device,MTL::PixelFormat format,MTL::TextureUsage usage ,bool mipmaped = false,MTL::StorageMode mode = MTL::StorageModeShared);
        
    };

}
