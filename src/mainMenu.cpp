#include "mainMenu.h"
#include "config.h"
#include "sharkGame.h"

const std::string MainMenu::BUTTON_NAMES[] = {"Start Game", "Exit"};
const int MainMenu::BUTTON_WIDTH = 220;
const int MainMenu::BUTTON_HEIGHT = 110;

void MainMenu::init(WindowState *window_state) {
    SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_RenderSetLogicalSize(gRenderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_ShowWindow(gWindow);
    State::init(window_state);
    SDL_RenderGetLogicalSize(gRenderer, &window_state->screen_width, &window_state->screen_height);
    SDL_GetRendererOutputSize(gRenderer, &window_state->window_width, &window_state->window_height);

    buttons.reserve(ButtonId::TOTAL);
    for (int i = 0; i < ButtonId::TOTAL; ++i) {
        buttons.emplace_back(
                (window_state->screen_width - BUTTON_WIDTH) / 2,
                (i + 1) * window_state->screen_height / 4,
                BUTTON_WIDTH,
                BUTTON_HEIGHT,
                BUTTON_NAMES[i], 40
        );
    }
    text.emplace_back(
            (window_state->screen_width / 2- 250),
            50,
            500,
            200,
            "FRUITSHARK",
            80
    );
    text.back().set_text_color(0x50, 0xe0, 0x50, 0xFF);
}

void MainMenu::resume() {
    State::resume();
    SDL_RenderSetViewport(gRenderer, nullptr);
}

void MainMenu::button_press(int btn) {
    switch (btn) {
        case ButtonId::START_GAME:
            next_res = {StateStatus::PUSH, new SharkGame()};
            break;
        case ButtonId::EXIT:
            menu_exit();
            break;
        default:
            break;
    }
}
