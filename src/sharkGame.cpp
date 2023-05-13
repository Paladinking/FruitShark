#include "sharkGame.h"
#include "config.h"

void SharkGame::init(WindowState* window_state) {
    SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_RenderSetLogicalSize(gRenderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_ShowWindow(gWindow);
    State::init(window_state);
    SDL_GetRendererOutputSize(gRenderer, &window_state->screen_width, &window_state->screen_height);
    SDL_GetWindowSize(gWindow, &window_state->window_width, &window_state->window_height);
    exit_input = get_hold_input("Escape");
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        ships.emplace_back(200.0 * (i + 1), 500.0, BINDINGS[i]);
    }
    create_shark_trails();
    for (unsigned  i = 0; i < INITIAL_SHARK_COUNT; ++i) {
        int x = engine::random(200, LOGICAL_WIDTH - 200);
        int y = engine::random(200, LOGICAL_HEIGHT - 200);
        sharks.emplace_back(x, y);
    }
}

void SharkGame::tick(Uint64 delta, StateStatus& res) {
    if (delta == 0) return;
    double dDelta = static_cast<double>(delta) / 1000.0;
    for (auto& ship : ships) {
        ship.tick(dDelta, window_state->keyboard_state, window_state->mouse_mask);
    }
    for (auto& shark : sharks) {
        shark.tick(dDelta);
    }

    if (exit_input->is_pressed(window_state->keyboard_state, window_state->mouse_mask)) {
        res.action = StateStatus::EXIT;
    }
}


void SharkGame::render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, nullptr);
    for (const auto& ship : ships) ship.render();
    for (const auto& shark : sharks) shark.render();
    SDL_RenderPresent(gRenderer);
}

void SharkGame::create_shark_trails() {
    shark_trails.push_back({
        {10.0, 10.0}, {20.0, 20.0}, {30.0, 30.0}
    });
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {

}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {

}


