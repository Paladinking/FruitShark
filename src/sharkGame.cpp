#include "sharkGame.h"
#include "config.h"
#include "asset_handlers/sound.h"
#include "engine/ui.h"

void Restarter::tick(Uint64 delt, StateStatus &res) {
    res.action = StateStatus::SWAP;
    res.new_state = new SharkGame();
}

Input::Input(const char *const *bindings) {
    forwards = get_hold_input(bindings[static_cast<Uint8>(Ship::Direction::FORWARDS)]);
    left = get_hold_input(bindings[static_cast<Uint8>(Ship::Direction::LEFT)]);
    right = get_hold_input(bindings[static_cast<Uint8>(Ship::Direction::RIGHT)]);
    left_cannon = get_press_input(bindings[4]);
    right_cannon = get_press_input(bindings[5]);
}

void Input::update(const Uint8 *keyboard, Uint32 mouse_mask) {
    hold_state[static_cast<Uint8>(Ship::Direction::FORWARDS)] =
            forwards->is_pressed(keyboard, mouse_mask);
    hold_state[static_cast<Uint8>(Ship::Direction::LEFT)] =
            left->is_pressed(keyboard, mouse_mask);
    hold_state[static_cast<Uint8>(Ship::Direction::RIGHT)] =
            right->is_pressed(keyboard, mouse_mask);
}

TextureHandler texture_handler = TextureHandler();

void SharkGame::init(WindowState* window_state) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    State::init(window_state);
    exit_input = get_hold_input("Escape");
    restart_input = get_hold_input("Space");
    initialize();
    for (const auto & i : BINDINGS) {
        inputs.emplace_back(i);
    }
    for (auto &i : inputs) {
        input_values.push_back(i.hold_state);
    }

    startup_textures[3] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "3", 64);
    startup_textures[2] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "2", 64);
    startup_textures[1] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "1", 64);
    startup_textures[0] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Go", 64);

    game_over[0] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Game Over", 64);
    game_over[1] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Press space to restart", 64);
}

void SharkGame::tick(Uint64 delta, StateStatus& res) {
    if (delta == 0) return;
    if (exit_input->is_pressed(window_state->keyboard_state, window_state->mouse_mask)) {
        res.action = StateStatus::POP;
    }
    double dDelta = static_cast<double>(delta) / 1000.0;
    if (startup_delay >= 0.0) {
        startup_delay -= dDelta;
        if (startup_delay <= 1.0) {
            state = PLAYING;
        } else if (state == STARTUP) return;
    }
    if (state == GAME_OVER) {
        if (restart_input->is_pressed(window_state->keyboard_state, window_state->mouse_mask)) {
            res.action = StateStatus::SWAP;
            res.new_state = new Restarter();
        }
        return;
    }
    for (auto& shark : sharks) {
        shark.tick_animation(dDelta);
    }
    for (int i = 0; i < bites.size(); ++i) {
        bites[i].tick(dDelta);
        if (bites[i].is_dead()) {
            bites[i] = bites[bites.size() - 1];
            bites.pop_back();
            --i;
        }
    }
    for (auto & input : inputs) {
        input.update(window_state->keyboard_state, window_state->mouse_mask);
    }
    tick_physics(dDelta, input_values);
}


void SharkGame::render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &game_viewport);
    for (const auto& ship : ships) ship.render();
    for (const auto& shark : sharks) shark.render();
    for (const auto& fruit : fruits_in_air) fruit.render();
    for (const auto& fruit : fruits_in_water) fruit.render();
    for (const auto& pickup : pickups) pickup.render();
    for (const auto& bite : bites) bite.render();

    if (startup_delay >= 0.0) {
        startup_textures[static_cast<int>(startup_delay)].render(0, 0);
    }
    if (state == GAME_OVER) {
        game_over[0].render(0, -200);
        game_over[1].render(0, 0);
        game_over[2].render(0, 200);
    }

    SDL_RenderPresent(gRenderer);
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {
    if (state != PLAYING) return;
    for (int i = 0; i < ships.size(); ++i) {
        ships[i].handle_up(
                inputs[i].left_cannon->is_targeted(key, mouse),
                inputs[i].right_cannon->is_targeted(key, mouse),
                *this);

    }
}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (state != PLAYING) return;
    for (int i = 0; i < ships.size(); ++i) {
        ships[i].handle_down(
                inputs[i].left_cannon->is_targeted(key, mouse),
                inputs[i].right_cannon->is_targeted(key, mouse));

    }
}

void SharkGame::cannon_fired(Vector2D position, Vector2D velocity, FruitType type) {
    sound::play(sound::Id::CANNON);
    fruits_in_air.emplace_back(position, velocity, type);
}

void SharkGame::ship_hurt(Vector2D position, int player_id, int dmg) {
    sound::play(sound::Id::BITE);
    bites.emplace_back(static_cast<int>(position.x), static_cast<int>(position.y));
}

void SharkGame::ship_destroyed(int id) {
    if (ships.size() == 1) {
        state = GAME_OVER;
        std::string msg = "Player " + std::to_string(ships[0].id + 1) + " Won!";
        game_over[2] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, msg, 64);
    }
}

void SharkGame::fruit_hit_water(int fruit, Vector2D position) {
    sound::play(sound::Id::WATER);
}

void SharkGame::fruit_hit_player(int fruit, int player_id) {}

void SharkGame::pickup_created(int x, int y, FruitType type) {
    pickups.emplace_back(x, y, type);
}


