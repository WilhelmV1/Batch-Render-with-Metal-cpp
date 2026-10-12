//
//  Shader.metal
//  Batch Render
//
//  Created by William Bustamante on 11/10/26.
//

#include <metal_stdlib>
using namespace metal;


struct UniformBuffer{
    float4x4 projection;
    float dt;
};


struct Vertex{
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    uint palette_index [[attribute(2)]];
    uint texture_index [[attribute(3)]];
};

struct QuadVertex {
    float3 position [[attribute(0)]];
    float3 color [[attribute(1)]];
};

struct QuadFragment {
    float4 position [[position]];
    float3 color;
};

struct Fragment {
    float4 position [[position]];
    float4 world_position;
    uint palette_index;
    uint texture_index;
    float2 uv;
};

[[vertex]] Fragment vertex_swap_chain_shader(Vertex in [[stage_in]]) {
    
    Fragment out;
    out.position = float4(in.position, 1.0f);
    out.uv = in.uv;
    out.palette_index = in.palette_index;
    out.texture_index = in.texture_index;
    
    return out;
}

[[vertex]]
Fragment vertex_shader(Vertex in [[stage_in]], constant UniformBuffer& ubo [[buffer(1)]])
{
    Fragment out;
    float4 world_position = float4(floor(in.position.xy), in.position.z, 1.0f);
    
    out.position = ubo.projection * world_position;
    
    out.uv = in.uv;
    out.palette_index = in.palette_index;
    out.texture_index = in.texture_index;
    
    return out;
}

[[vertex]]
QuadFragment quad_vertex_shader(QuadVertex in [[stage_in]], constant UniformBuffer& ubo [[buffer(1)]])
{
    QuadFragment out;
    float4 world_position = float4(floor(in.position.xy), in.position.z, 1.0f);
    
    out.position = ubo.projection * world_position;
    out.color = in.color;
    
    return out;
}

[[fragment]]
half4 final_fragment_shader(Fragment frag [[stage_in]], texture2d<float> texture [[texture(0)]]) {
    
    constexpr sampler s{filter::nearest, coord::normalized, address::clamp_to_edge};
    return half4(texture.sample(s, frag.uv));
}


[[fragment]]
half4 fragment_shader(const Fragment in [[stage_in]], constant UniformBuffer& ubo [[buffer(1)]] ,device const uint8_t* palette_colors [[buffer(2)]],const device float4* colors [[buffer(3)]] ,texture2d_array<uint, access::read> texture [[texture(0)]]){
    
    uint2 pixel_coord = uint2(in.uv.x, in.uv.y);
    
    uint color_index = texture.read(pixel_coord, in.texture_index).r;
    uint palette_offset = in.palette_index * 4 + color_index;
    palette_offset = clamp(palette_offset, 0u, 64u);
    uint index = palette_colors[palette_offset];
    float4 nes_rgb = colors[index];
    
    if ((in.palette_index <= 1 && color_index == 3) || (in.palette_index > 1 && in.palette_index < 12 && color_index == 0) || in.palette_index > 12 && color_index == 0) {
        discard_fragment();
    }
 
    return half4(nes_rgb);
   
}

[[fragment]]
half4 quad_fragment_shader(const QuadFragment in [[stage_in]],
                           constant UniformBuffer& ubo [[buffer(1)]])
{
    // UV en [0,1] a partir de la posición NDC (que viene en [-1,1])
    float2 uv = in.position.xy / float2(800.0, 600.0);
    
    // Escalar para que el plasma no sea demasiado denso
    float2 p = uv * 6.0;
    float t = ubo.dt;
    
    // Plasma clásico: suma de senos
    float v = 0.0;
    v += sin(p.x + t);
    v += sin(p.y * 0.8 + t * 1.3);
    v += sin((p.x + p.y) * 0.7 + t * 0.9);
    v += sin(length(p - float2(3.0, 3.0)) * 1.5 - t * 1.8);
    v += sin(length(p - float2(-3.0, 3.0)) * 1.5 + t * 1.1);
    v *= 0.2;   // promedio
    
    // Mapear a color: el seno va de -1 a 1, lo pasamos a [0,1]
    float r = sin(v * 3.14159) * 0.5 + 0.5;
    float g = sin(v * 3.14159 + 2.094) * 0.5 + 0.5;   // desfase 120°
    float b = sin(v * 3.14159 + 4.188) * 0.5 + 0.5;   // desfase 240°
    
    // Mezclar con el color del vértice para que no sea monótono
    float3 plasma = float3(r, g, b);
    float3 final_color = mix(in.color * 0.5, plasma, 0.85);
    
    return half4(half3(final_color), 1.0);
}
