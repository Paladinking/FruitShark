#include "sharkGame.h"
#include "config.h"

void SharkGame::tick(Uint64 delta, StateStatus& res) {
    if (delta == 0) return;
    double dDelta = static_cast<double>(delta) / 1000.0;
    for (auto& ship : ships) {
        ship.tick(dDelta, window_state->keyboard_state, window_state->mouse_mask);
    }

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
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        ships.emplace_back(200.0 * (i + 1), 500.0, BINDINGS[i]);
    }
}

void SharkGame::render() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    for (const auto& ship : ships) ship.render();
    SDL_RenderPresent(gRenderer);
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {

}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {

}