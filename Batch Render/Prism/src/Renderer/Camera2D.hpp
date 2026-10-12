//
//  Camera2D.hpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//
#pragma once
#include <simd/simd.h>

namespace prism {

class Camera2D{
public:
    
    void set_orthographic_projection(float left, float right, float top, float bottom, float near, float far);
    void set_view_matrix(simd::float3  position, simd::float3 rotation);
    
    const simd::float4x4& get_ortho_matrix() const;
    const simd::float4x4& get_view_matrix() const;
    const simd::float3 get_position() const;

private:
    simd::float4x4 ortho_matrix{1.0f};
    simd::float4x4 view_matrix{1.0f};
    simd::float4x4 inverse_view_matrix{1.0f};
};


}
