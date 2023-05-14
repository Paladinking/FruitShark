#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "engine/game.h"
#include "mainMenu.h"
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
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

/**
 * Initialize SDL, engine and config.
 */
void init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "Could not initialize SDL, "  << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (TTF_Init() != 0) {
        std::cout << "Could not initialize SDL_tff, " << TTF_GetError() << std::endl;
        exit(-2);
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::cout << "Could not initialize SDL_mixer, " << Mix_GetError() << std::endl;
        exit(-3);
    }

    try {
        engine::init();
    } catch (const base_exception &e) {
        std::cout << e.msg << std::endl;
        exit(-4);
    }
}

int main(int argc, char* args[])
{
    atexit(cleanup);

    init();

    StateGame game(new MainMenu(), 100, 100, "SharkGame");
    int exit_status = 0;
    try {
        game.create();
        game.run();
    } catch (const base_exception &e) {
        std::cout << e.msg << std::endl;
        exit_status = -1;
    } catch (const std::logic_error &e) {
        std::cout << e.what() << std::endl;
        exit_status = -2;
    }

    return exit_status;
}
