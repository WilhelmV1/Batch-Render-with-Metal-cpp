//
//  BatchRenderer.hpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once

#include <simd/simd.h>

namespace MTL {

class ResidencySet;

}

namespace prism {

struct FrameContext;
class Device;

class BatchRender {
public:
    
    static void init(Device& device, MTL::ResidencySet* residency);
    static void shutdown();
    
    static void begin_batch(const FrameContext& context);
    static void present();
        
    static void draw_sprite(float x, float y , float z ,float w, float h, simd::float2 sprite_pos, simd::float2 sprite_size, simd::uint1 palette_index, simd::uint1 texture_index);
    
    static void draw_quad(float x, float y , float z ,float w, float h, simd::float3 color);
    static void draw_multicolor_quad(float x, float y , float z ,float w, float h, simd::float3 color1, simd::float3 color2, simd::float3 color3, simd::float3 color4);
    
private:
    
    static void push_vertex(simd::float3 position ,simd::float2 uv, simd::uint1 palette_index, simd::uint1 texture_index);
    static void push_vertex(simd::float3 position, simd::float3 color);
    static void flush();
    static void reset();
};

}
