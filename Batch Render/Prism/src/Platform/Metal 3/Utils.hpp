//
//  Utils.hpp
//  Batch Render
//
//  Created by William Bustamante on 10/10/26.
//
#pragma once
#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <string>

namespace prism {

inline std::string to_string(NS::String* string){
    if (!string) return "";
    
    const char* c_str = string->utf8String();
    return c_str ? std::string(c_str) : std::string("");
 }

inline NS::String* to_ns_string(std::string_view text){
    
    if(text.empty()) return NS::String::alloc()->init("", NS::UTF8StringEncoding);
    
    return NS::String::alloc()->init(text.data(), NS::UTF8StringEncoding);
   
}

inline std::string resourcePath(){
    std::string path;
    NS::Bundle* bundle = NS::Bundle::mainBundle();
    assert(bundle && "There is not bundle");
    NS::String* NSBundlePath = bundle->resourcePath();
    path = NSBundlePath->utf8String() + std::string{"/"};
    return path;
}



}
