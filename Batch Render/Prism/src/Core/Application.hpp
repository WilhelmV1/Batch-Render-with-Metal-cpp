//
//  Application.hpp
//  Batch Render
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once

#include "Window.hpp"
#include "Layer.hpp"
#include "Device.hpp"

namespace prism {

class App {
public:
    
    App();
    ~App();
    
    void run();
    
    
private:
    
    Window window{};
    Device device{};
    Layer layer{window, device};
    
};

}
