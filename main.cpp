#include <iostream>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <SDL2/SDL.h>
// #include <C:/KUNSKAP/CODING_TRAINING/C++/berkley_sockets/lib/SDL2/x86_64-w64-mingw32/include/SDL2/SDL.h>

int wmain(int, char**) {

	// Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

	// Pause program execution for a certain duration
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // 1000 milliseconds = 1 second

	// Cleanup SDL
    SDL_Quit();

    return 0;
}
