//
//  Window.cpp
//  Super Mario Bros 3
//
//  Created by William Bustamante on 05/10/26.
//

#include "Window.hpp"

#include <stdexcept>
#include <string>

namespace prism {

Window::Window() {
    init();
}

Window::~Window() {
    if (window) SDL_DestroyWindow(window);
}

bool Window::is_open() const  {
    return is_running;
}

void Window::close() {
    is_running = false;
}

void Window::init() {
    
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))  throw std::runtime_error("Failed to init SDL3: " + std::string(SDL_GetError()));
    
    window = SDL_CreateWindow("Super Mario Bros 3", 800, 600, SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE);
    
    if (!window) throw std::runtime_error("Failed to create Window: " + std::string(SDL_GetError()));
    
    is_running = true;
}

}
