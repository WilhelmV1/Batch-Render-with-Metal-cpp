//
//  ShaderTypes.hpp
//  Batch Render
//
//  Created by William Bustamante on 10/10/26.
//
#pragma once
#include <simd/simd.h>


namespace prism {

struct Vertex {
    simd::float3 position;
    simd::float2 uv;
    simd::uint1 palette_index;
    simd::uint1 texture_index;
};

struct QuadVertex {
    simd::float3 position;
    simd::float3 color;
};

struct UniformBuffer {
    simd::float4x4 projection;
    float dt;
};


}
