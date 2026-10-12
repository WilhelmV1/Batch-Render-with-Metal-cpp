//
//  mtl_texture.cpp
//  Super Mario Bros 2
//
//  Created by William Bustamante on 28/03/26.
//

#include "MetalTexture.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <format>

#include "stb_image.hpp"
#include "Utils.hpp"


namespace prism {


pixels* Texture::load_texture(std::string_view filename){
    
    if (filename.empty()) return nullptr;
    
    std::string bundlePath = resourcePath() + filename.data();
    
    int channels;
    
    int w, h;
   
    
    stbi_uc* img = stbi_load(bundlePath.c_str(), &w, &h, &channels, 1);
    
    if (!img) {
        std::cerr<<"Failed to load Image \n";
        return nullptr;
    }
    
    width = w;
    height = h;
    
    return img;
}
void Texture::make_resident(MTL::ResidencySet& set) const{
    set.addAllocation(texture);
}

void Texture2D::load_texture_from_file(std::string_view filename, Device &device, MTL::PixelFormat format, MTL::TextureUsage usage, bool mipmaped, MTL::StorageMode mode){
    
    pixels* pixels = load_texture(filename);
    
    MTL::TextureDescriptor* textDescriptor = MTL::TextureDescriptor::texture2DDescriptor(format, width, height, mipmaped);
    
    textDescriptor->setStorageMode(mode);
    textDescriptor->setAllowGPUOptimizedContents(true);
    textDescriptor->setUsage(usage);
   
    
    auto msg = std::format("Mip Map Count: {} \n", textDescriptor->mipmapLevelCount());
  
    std::cout<<msg;
 
    
   
    
    texture = device.get_device()->newTexture(textDescriptor);
    
    try {
        if (!texture) throw std::runtime_error("Failed to create texture");
    }catch(const std::exception& error){
        std::cout<<"Error: "<<error.what()<<"\n";
    }
    
   
    
    NS::UInteger bytesPerRow = width;
    MTL::Region region(0, 0, width, height);
    texture->replaceRegion(region, 0, pixels, bytesPerRow);
    textDescriptor->release();
    stbi_image_free(pixels);
    
}

void Texture2D::create_depth_texture(Device &device, MTL::PixelFormat format, simd::float2 size){
    
    MTL::TextureDescriptor* texture_descriptor = MTL::TextureDescriptor::texture2DDescriptor(format, size.x, size.y, false);
    
    texture_descriptor->setStorageMode(MTL::StorageModeShared);
    texture_descriptor->setAllowGPUOptimizedContents(true);
    texture_descriptor->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
    
    texture = device.get_device()->newTexture(texture_descriptor);
    
    try {
        if (!texture) throw std::runtime_error("Failed to create texture");
    }catch(const std::exception& error){
        std::cout<<"Error: "<<error.what()<<"\n";
    }
    
    texture_descriptor->release();
    
}


/**
    @param width  you must select an defualt height for your Sprite Sheet
    @param height you must select an defualt height for your Sprite Sheet
 */

void ArrayTexture2D::load_texture_from_files(std::vector<std::string_view> paths, int width, int height ,Device &device, MTL::PixelFormat format, MTL::TextureUsage usage, bool mipmaped, MTL::StorageMode mode){
    
    
    MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
    
    descriptor->setArrayLength(paths.size());
    descriptor->setUsage(usage);
    descriptor->setPixelFormat(format);
    descriptor->setStorageMode(mode);
    descriptor->setAllowGPUOptimizedContents(true);
    descriptor->setWidth(width);
    descriptor->setHeight(height);
    descriptor->setTextureType(MTL::TextureType2DArray);
    
    texture = device.get_device()->newTexture(descriptor);
    
    try {
        if (!texture) throw std::runtime_error("Failed to create texture");
    }catch(const std::exception& error){
        std::cout<<"Error: "<<error.what()<<"\n";
    }
    
    
    for (int i = 0; i < paths.size(); i++) {
        
        auto pixels = load_texture(paths[i]);
        
        MTL::Region region = MTL::Region::Make2D(0, 0, this->width, this->height);
    
        texture->replaceRegion(region, 0, i, pixels, this->width, this->width * this->height);
        
        stbi_image_free(pixels);
    }
    
    descriptor->release();
   
    
}


}

