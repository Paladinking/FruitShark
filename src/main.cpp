#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "engine/game.h"
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
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

/**
 * Initialize SDL, engine and config.
 */
void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Could not initialize SDL, "  << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (TTF_Init() != 0) {
        std::cout << "Could not initialize SDL_tff, " << TTF_GetError() << std::endl;
        exit(-2);
    }

    try {
        engine::init();
    } catch (const base_exception &e) {
        std::cout << e.msg << std::endl;
        exit(-3);
    }
}

int main(int argc, char* args[])
{
    atexit(cleanup);

    init();

    Game game(500, 500, "FruitShark");
    game.create();
    SDL_ShowWindow(gWindow);
    game.run();

    int exit_status = 0;

    return exit_status;
}
