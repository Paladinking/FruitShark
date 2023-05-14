#include "sharkGame.h"
#include "config.h"
#include "sound.h"

TextureHandler textureHandler = TextureHandler();

void SharkGame::init(WindowState* window_state) {
    textureHandler.loadTextures();
    sound::play(sound::WATER);
    State::init(window_state);
    exit_input = get_hold_input("Escape");
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        ships.emplace_back(200.0 * (i + 1), 500.0, BINDINGS[i]);
    }
    create_shark_trails();
    for (unsigned  i = 0; i < INITIAL_SHARK_COUNT; ++i) {
        int x = engine::random(200, LOGICAL_WIDTH - 200);
        int y = engine::random(200, LOGICAL_HEIGHT - 200);
        int path = engine::random(0, static_cast<Sint32>(shark_trails.size()));
        sharks.emplace_back(x, y);
        sharks[i].set_trail(&shark_trails[path]);
    }
}

void SharkGame::tick(Uint64 delta, StateStatus& res) {
    if (delta == 0) return;
    double dDelta = static_cast<double>(delta) / 1000.0;
    for (auto& ship : ships) {
        ship.tick(dDelta, window_state->keyboard_state, window_state->mouse_mask, fruitsInAir);
    }
    for (auto& shark : sharks) {
        shark.tick(dDelta, shark_trails, fruitsInWater);
    }

    for (int i = 0; i < ships.size(); ++i) {
        for (int j = i + 1; j < ships.size(); ++j) {
            ships[i].handle_collision(ships[j]);
        }
    }

    for (auto &shark : sharks) {
        for (auto &ship : ships) {
            if (ship.intersects(shark)) {
                shark.bite(ship);
            }
        }
    }

    for (int i = 0; i < fruitsInAir.size(); ++i) {
        auto& fruit = fruitsInAir[i];
        fruit.tick(dDelta);
        if (fruit.inWater) {
            fruitsInWater.emplace_back(fruitsInAir[i]);
            fruitsInAir[i] = fruitsInAir[fruitsInAir.size() - 1];
            fruitsInAir.pop_back();
            --i;
        }
    }

    for (int i = 0; i < fruitsInWater.size(); ++i) {
        auto& fruit = fruitsInWater[i];
        fruit.tick(dDelta);
        if (fruit.eaten) {
            fruitsInWater[i] = fruitsInWater[fruitsInWater.size() - 1];
            fruitsInWater.pop_back();
            --i;
        }
    }

    if (exit_input->is_pressed(window_state->keyboard_state, window_state->mouse_mask)) {
        res.action = StateStatus::POP;
    }
}


void SharkGame::render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &game_viewport);
    for (const auto& ship : ships) ship.render();
    for (const auto& shark : sharks) shark.render();
    for (const auto& fruit : fruitsInAir) fruit.render();
    for (const auto& fruit : fruitsInWater) fruit.render();

    SDL_RenderPresent(gRenderer);
}

void SharkGame::create_shark_trails() {
    shark_trails.push_back({
        {100.0, 100.0}, {430.0, 100.0}, {430.0, 430.0}, {100.0, 430.0}
    });
    shark_trails.push_back({
        {530.0, 100.0}, {530.0, 430.0}, {860.0, 430.0}, {860.0, 100.0}
    });
    shark_trails.push_back({
        {960.0, 100.0}, {960.0, 430.0}, {1290.0, 430.0}, {1290.0, 100.0}
    });
    shark_trails.push_back({
        {1390.0, 100.0}, {1720.0, 100.0}, {1720.0, 430.0}, {1390.0, 430.0}
    });

    shark_trails.push_back({
        {100.0, 500.0}, {430.0, 500.0}, {430.0, 900.0}, {100.0, 900.0}
    });
    shark_trails.push_back({
        {530.0, 500.0}, {530.0, 900.0}, {860.0, 900.0}, {860.0, 500.0}
    });
    shark_trails.push_back({
        {960.0, 500.0}, {960.0, 900.0}, {1290.0, 900.0}, {1290.0, 500.0}
    });
    shark_trails.push_back({
        {1390.0, 500.0}, {1720.0, 500.0}, {1720.0, 900.0}, {1390.0, 900.0}
    });
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {
    for (auto& ship : ships) {
        ship.handle_up(key, mouse, fruitsInAir);
    }
}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {
    for (auto& ship : ships) {
        ship.handle_down(key, mouse);
    }
}


