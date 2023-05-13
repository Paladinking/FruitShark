#include "sharkGame.h"
void SharkGame::tick(Uint64 delta, StateStatus& res) {
    if (exit_input->is_pressed(window_state->keyboard_state, window_state->mouse_mask)) {
        res.action = StateStatus::EXIT;
    }
}

void SharkGame::init(WindowState* window_state) {
    SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_ShowWindow(gWindow);
    State::init(window_state);
    SDL_GetRendererOutputSize(gRenderer, &window_state->screen_width, &window_state->screen_height);
    SDL_GetWindowSize(gWindow, &window_state->window_width, &window_state->window_height);
    exit_input = get_hold_input("Escape");
}

void SharkGame::render() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_RenderPresent(gRenderer);
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {

}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {

}