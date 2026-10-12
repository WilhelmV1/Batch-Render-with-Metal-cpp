//
//  Window.hpp
//  Super Mario Bros 3
//
//  Created by William Bustamante on 05/10/26.
//
#pragma once
#include <SDL3/SDL.h>
 
namespace prism {

class Window {
public:
    
    Window();
    ~Window();
    
    bool is_open() const;
    void close();
    SDL_Window* get_window() { return window; };
    
private:
    
    void init();
    
    SDL_Window* window = nullptr;
    bool is_running;
    
};

}
