//
// Created by axelh on 2023-05-20.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <enet/enet.h>
#include "engine/game.h"
#include "mainMenu.h"
#include "net/server.h"
/**
 * Frees still used global resources and quits SDL and SDL_image.
 * Called by atexit().
 **/
void cleanup()
{
    if (gRenderer != nullptr)
    {
        std::cout << "Destroying renderer" << std::endl;
        SDL_DestroyRenderer(gRenderer);
    }
    if (gWindow != nullptr)
    {
        std::cout << "Destroying window" << std::endl;
        SDL_DestroyWindow(gWindow);
    }
    std::cout << "Shutting down..." << std::endl;
    SDL_Quit();
    enet_deinitialize();
}

/**
 * Initialize SDL, engine and config.
 */
void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Could not initialize SDL, "  << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (enet_initialize() < 0) {
        std::cout << "Could not initialize enet" << std::endl;
        exit(-4);
    }
}

int main(int argc, char* args[])
{
    atexit(cleanup);

    init();
    int exit_status = 0;
    Server server;
    try {
        server.headless();
        server.run_headless();
    } catch (const base_exception &e) {
        std::cout << e.msg << std::endl;
        exit_status = -1;
    } catch (const std::logic_error &e) {
        std::cout << e.what() << std::endl;
           exit_status = -2;
    }
    exit(exit_status);
}
