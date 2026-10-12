//
//  Palette.hpp
//  Batch Render
//
//  Created by William Bustamante on 10/10/26.
//
#pragma once
#include <simd/simd.h>

namespace NES {


enum ColorType : std::uint8_t{
    COLOR_00,
    COLOR_01,
    COLOR_02,
    COLOR_03,
    COLOR_04,
    COLOR_05,
    COLOR_06,
    COLOR_07,
    COLOR_08,
    COLOR_09,
    COLOR_0A,
    COLOR_0B,
    C0LOR_0C,
    COLOR_0D,
    
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15,
    COLOR_16,
    COLOR_17,
    COLOR_18,
    COLOR_19,
    COLOR_1A,
    COLOR_1B,
    C0LOR_1C,
    COLOR_1D,
    
    COLOR_20,
    COLOR_21,
    COLOR_22,
    COLOR_23,
    COLOR_24,
    COLOR_25,
    COLOR_26,
    COLOR_27,
    COLOR_28,
    COLOR_29,
    COLOR_2A,
    COLOR_2B,
    C0LOR_2C,
    COLOR_2D,
    
    COLOR_30,
    COLOR_31,
    COLOR_32,
    COLOR_33,
    COLOR_34,
    COLOR_35,
    COLOR_36,
    COLOR_37,
    COLOR_38,
    COLOR_39,
    COLOR_3A,
    COLOR_3B,
    C0LOR_3C,
    COLOR_3D,
    
    
    COLOR_COUNT
};

constexpr inline simd::float4 make_nes_color(float r, float g, float b, float a = 1.0f)  {
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;
    return simd::float4{r,g,b,a};
};



constexpr simd::float4 colors[(int)ColorType::COLOR_COUNT]{
    
    make_nes_color(121.0f, 121.0f, 121.0f), // COLOR 00
    make_nes_color(0.0f  , 42.0f , 178.0f), // COLOR 01
    make_nes_color(40.0f , 0.0f  , 186.0f), // COLOR 02
    make_nes_color(97.0f , 16.0f , 162.0f), // COLOR 03
    make_nes_color(154.0f, 32.0f , 121.0f), // COLOR 04
    make_nes_color(181.0f, 49.0f , 32.0f),  // COLOR 05
    make_nes_color(162.0f, 48.0f , 0.0f),   // COLOR 06
    make_nes_color(121.0f, 65.0f , 0.0f),   // COLOR 07
    make_nes_color(73.0f , 89.0f , 0.0f),   // COLOR 08
    make_nes_color( 0.0f ,  82.0f, 0.0f),   // COLOR 09
    make_nes_color(56.0f , 109.0f, 0.0f),   // COLOR 0A
    make_nes_color(48.0f , 97.0f , 65.0f),  // COLOR 0B
    make_nes_color(48.0f , 81.0f , 130.0f), // COLOR 0C
    make_nes_color(0.0f  , 0.0f  , 0.0f, 0.0f), // COLOR 0D
    
    
    make_nes_color(178.0f, 178.0f, 178.0f), // COLOR 10
    make_nes_color(65.0f , 97.0f , 251.0f), // COLOR 11
    make_nes_color(65.0f , 65.0f , 255.0f), // COLOR 12
    make_nes_color(146.0f, 65.0f , 243.0f), // COLOR 13
    make_nes_color(219.0f, 65.0f , 195.0f), // COLOR 14
    make_nes_color(219.0f, 65.0f , 97.0f),  // COLOR 15
    make_nes_color(227.0f, 81.0f , 0.0f),   // COLOR 16
    make_nes_color(195.0f, 113.0f, 0.0f),   // COLOR 17
    make_nes_color(138.0f, 138.0f, 0.0f),   // COLOR 18
    make_nes_color(12.0f , 147.0f, 0.0f),   // COLOR 19
    make_nes_color(73.0f , 170.0f, 16.0f),  // COLOR 1A
    make_nes_color(73.0f , 162.0f, 105.0f), // COLOR 1B
    make_nes_color(65.0f , 146.0f, 195.0f), // COLOR 1C
    make_nes_color(0.0f  , 0.0f  , 0.0f),   // COLOR 1D
    
    
    make_nes_color(235.0f, 235.0f, 235.0f), // COLOR 20
    make_nes_color(97.0f, 162.0f, 255.0f),  // COLOR 21
    make_nes_color(81.0f, 130.0f, 255.0f),  // COLOR 22
    make_nes_color(162.0f, 113.0f, 255.0f), // COLOR 23
    make_nes_color(243.0f, 97.0f, 255.0f),  // COLOR 24
    make_nes_color(255.0f, 97.0f, 178.0f),  // COLOR 25
    make_nes_color(255.0f, 121.0f, 48.0f),  // COLOR 26
    make_nes_color(234.0f, 158.0f, 34.0f),  // COLOR 27
    make_nes_color(235.0f, 211.0f, 32.0f),  // COLOR 28
    make_nes_color(136.0f, 216.0f, 0.0f),   // COLOR 29
    make_nes_color(113.0f, 243.0f, 65.0f),  // COLOR 2A
    make_nes_color(113.0f, 227.0f, 146.0f), // COLOR 2B
    make_nes_color(97.0f, 211.0f, 227.0f),  // COLOR 2C
    make_nes_color(121.0f, 121.0f, 121.0f), // COLOR 2D
    
    
    make_nes_color(255.0f, 255.0f, 255.0f), // COLOR 30
    make_nes_color(146.0f, 211.0f, 255.0f), // COLOR 31
    make_nes_color(162.0f, 186.0f, 0.0f),   // COLOR 32
    make_nes_color(195.0f, 178.0f, 255.0f), // COLOR 33
    make_nes_color(227.0f, 178.0f, 255.0f), // COLOR 34
    make_nes_color(255.0f, 186.0f, 235.0f), // COLOR 35
    make_nes_color(255.0f, 203.0f, 186.0f), // COLOR 36
    make_nes_color(255.0f, 219.0f, 162.0f), // COLOR 37
    make_nes_color(255.0f, 243.0f, 146.0f), // COLOR 38
    make_nes_color(203.0f, 245.0f, 130.0f), // COLOR 39
    make_nes_color(162.0f, 243.0f, 162.0f), // COLOR 3A
    make_nes_color(162.0f, 255.0f, 203.0f), // COLOR 3B
    make_nes_color(162.0f, 255.0f, 243.0f), // COLOR 3C
    make_nes_color(162.0f, 162.0f, 162.0f), // COLOR 3D
    
};

struct Palette{
    uint8_t palette[4];
};


}
