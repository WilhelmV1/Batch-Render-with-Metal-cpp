//
//  Camera2D.cpp
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//

#include "Camera2D.hpp"


namespace prism {


void Camera2D::set_orthographic_projection(float left, float right, float top, float bottom, float near, float far){
    
    simd::float4 col0 = {2.f / (right - left) , 0.0f, 0.0f, 0.0f};
    simd::float4 col1 = {0.0f, 2.f / (top - bottom), 0.0f, 0.0f};
    simd::float4 col2 = {0.0f, 0.0f,  1.f / (far - near), 0.0f};
    simd::float4 col3 = {-(right + left) / (right - left), -(top + bottom) / (top - bottom), -near / (far - near), 1.0f};
    
    ortho_matrix = simd_matrix(col0, col1, col2, col3);
}

const simd::float4x4& Camera2D::get_ortho_matrix() const {
    return ortho_matrix;
}

void Camera2D::set_view_matrix(simd::float3 position, simd::float3 rotation) {

    const float c3 = simd::cos(rotation.z);
    const float s3 = simd::sin(rotation.z);
    const float c2 = simd::cos(rotation.x);
    const float s2 = simd::sin(rotation.x);
    const float c1 = simd::cos(rotation.y);
    const float s1 = simd::sin(rotation.y);
    const simd::float3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const simd::float3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const simd::float3 w{(c2 * s1), (-s2), (c1 * c2)};

    simd::float4 col0 = {u.x, v.x, w.x, 0.0f};
    simd::float4 col1 = {u.y, v.y, w.y, 0.0f};
    simd::float4 col2 = {u.z, v.z, w.z, 0.0f};
    simd::float4 col3 = {-simd::dot(u, position), -simd::dot(v, position), -simd::dot(w, position), 1.0f};

    view_matrix = simd_matrix(col0, col1, col2, col3);

    simd::float4 col4 = {u.x, u.y, u.z, 0.0f};
    simd::float4 col5 = {v.x, v.y, v.z, 0.0f};
    simd::float4 col6 = {w.x, w.y, w.z, 0.0f};
    simd::float4 col7 = {position.x, position.y, position.z, 1.0f};

    inverse_view_matrix = simd_matrix(col4, col5, col6, col7);


}
const simd::float4x4& Camera2D::get_view_matrix() const {
    return view_matrix;
}

const simd::float3 Camera2D::get_position() const {return simd_make_float3(inverse_view_matrix.columns[3].x,inverse_view_matrix.columns[3].y, inverse_view_matrix.columns[3].z );
}



}
