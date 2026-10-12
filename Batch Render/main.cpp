//
//  main.cpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <Foundation/Foundation.hpp>

#include <SDL3/SDL_main.h>

#include <memory>

#include "Application.hpp"

int main(int argc, char * argv[]) {
    
    std::unique_ptr<prism::App> app = std::make_unique<prism::App>();
    
    app->run();
    
    return 0;
}

