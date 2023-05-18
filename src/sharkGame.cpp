#include "sharkGame.h"
#include "config.h"
#include "sound.h"
#include "engine/ui.h"

void Restarter::tick(Uint64 delt, StateStatus &res) {
    res.action = StateStatus::SWAP;
    res.new_state = new SharkGame();
}

TextureHandler textureHandler = TextureHandler();

void SharkGame::init(WindowState* window_state) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    sound::play(sound::WATER);
    State::init(window_state);
    exit_input = get_hold_input("Escape");
    restart_input = get_hold_input("Space");
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        ships.emplace_back(START_X[PLAYER_COUNT - 1][i], START_Y[PLAYER_COUNT - 1][i], BINDINGS[i],
                           COLORS[i], i, PI * i);
    }
    create_shark_trails();
    for (unsigned  i = 0; i < INITIAL_SHARK_COUNT; ++i) {
        int x, y;
        bool good;
        do {
            good = true;
            x = engine::random(200, LOGICAL_WIDTH - 200);
            y = engine::random(200, LOGICAL_HEIGHT - 200);
            for(auto& ship : ships) {
                if (ship.get_position().distance_squared({
                    static_cast<double>(x), static_cast<double>(y)
                }) < 40000.0) {
                    good = false;
                    break;
                }
            }
        } while(!good);
        int path = engine::random(0, static_cast<Sint32>(shark_trails.size()));
        int type = engine::random(0, 100);
        if (type > 50) {
            sharks.push_back(Shark::create_shark(Shark::Type::GREAT_WHITE, x, y));
        } else {
            sharks.push_back(Shark::create_shark(Shark::Type::TIGER, x, y));
        }

        sharks[i].set_trail(&shark_trails[path]);
    }
    sharks.push_back(Shark::create_shark(Shark::Type::MEGA, LOGICAL_WIDTH / 2, LOGICAL_HEIGHT / 2));

    startup_textures[3] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "3", 64);
    startup_textures[2] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "2", 64);
    startup_textures[1] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "1", 64);
    startup_textures[0] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Go", 64);

    game_over[0] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Game Over", 64);
    game_over[1] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, "Press space to restart", 64);

    pickup_delay = PICKUP_SPAWN_TIME;
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
    for (auto& ship : ships) {
        ship.tick(dDelta, window_state->keyboard_state, window_state->mouse_mask, fruits_in_air);
    }
    for (auto& shark : sharks) {
        shark.tick(dDelta, shark_trails, fruits_in_water, ships);
    }

    for (int i = 0; i < bites.size(); ++i) {
        bites[i].tick(dDelta);
        if (bites[i].is_dead()) {
            bites[i] = bites[bites.size() - 1];
            bites.pop_back();
            --i;
        }
    }

    for (int i = 0; i < ships.size(); ++i) {
        for (int j = i + 1; j < ships.size(); ++j) {
            if (ships[i].intersects(ships[j])) {
                ships[i].handle_Collision(ships[j]);
            }
        }
        for (int j = 0; j < pickups.size(); ++j) {
            if (ships[i].intersects(pickups[j].get_position(), pickups[j].get_radius())) {
                ships[i].add_fruits(pickups[j].getType(), 4);
                pickups.erase(pickups.begin() + j);
                --j;
            }
        }
    }

    for (auto &shark : sharks) {
        for (int i = 0; i < ships.size(); ++i) {
            if (ships[i].intersects(shark)) {
                if (shark.bite(ships[i])) {
                    Vector2D pos = shark.get_mouth();
                    bites.emplace_back(pos.x, pos.y);
                    if (ships[i].is_dead()) {
                        ships.erase(ships.begin() + i);
                        --i;
                        if (ships.size() == 1) {
                            state = GAME_OVER;
                            std::string msg = "Player " + std::to_string(ships[0].id + 1) + " Won!";
                            game_over[2] = TextBox(UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT, msg, 64);
                            break;
                        }
                    }
                }
                ships[i].handle_Collision(shark);
            }
        }
    }

    std::vector<Fruit> to_be_added;
    for (int i = 0; i < fruits_in_air.size(); ++i) {
        auto& fruit = fruits_in_air[i];
        fruit.tick(dDelta, to_be_added);
        bool collision = false;
        for (auto& ship : ships) {
            if (ship.intersects(fruit.get_position(), fruit.get_radius())) {
                ship.add_fruit_smell(fruit.get_duration());
                fruits_in_air[i] = fruits_in_air[fruits_in_air.size() - 1];
                fruits_in_air.pop_back();
                --i;
                collision = true;
                break;
            }
        }
        if (collision) continue;

        if (fruit.inWater) {
            sound::play(sound::Id::WATER);
            if (fruit.get_position().x >= UI_SIZE && fruit.get_position().x < LOGICAL_WIDTH - UI_SIZE
                && fruit.get_position().y >= 0 && fruit.get_position().y < LOGICAL_HEIGHT) {
                fruits_in_water.emplace_back(fruits_in_air[i]);
            }
            fruits_in_air[i] = fruits_in_air[fruits_in_air.size() - 1];
            fruits_in_air.pop_back();
            --i;
        }
    }

    for (auto& fruit : to_be_added) {
        fruits_in_air.push_back(fruit);
    }

    for (int i = 0; i < fruits_in_water.size(); ++i) {
        auto& fruit = fruits_in_water[i];
        fruit.tick(dDelta, to_be_added);
        for (auto& shark : sharks) {
            if (shark.intersects(fruit.get_position(), fruit.get_radius())) {
                fruit.eaten = true;
                break;
            }
        }
        if (fruit.eaten) {
            fruits_in_water[i] = fruits_in_water[fruits_in_water.size() - 1];
            fruits_in_water.pop_back();
            --i;
        }
    }

    for (auto& pickup : pickups) {
        pickup.tick(dDelta);
    }

    pickup_delay -= dDelta;
    if(pickup_delay < 0.0) {
        pickup_delay = PICKUP_SPAWN_TIME;
        create_pickup();
    }
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

void SharkGame::create_pickup() {
    int x = engine::random(UI_SIZE * 2, GAME_WIDTH - UI_SIZE * 2);
    int y = engine::random(UI_SIZE, GAME_HEIGHT - UI_SIZE);
    FruitType possible_fruits[] = {FruitType::BANANA, FruitType::POMEGRANATE};
    FruitType fruit_pickup = possible_fruits[engine::random(0, 2)];
    pickups.emplace_back(x, y, fruit_pickup);
}

void SharkGame::create_shark_trails() {
    shark_trails.push_back({
        {150.0, 100.0}, {430.0, 100.0}, {430.0, 430.0}, {150.0, 430.0}
    });
    shark_trails.push_back({
        {530.0, 100.0}, {530.0, 430.0}, {860.0, 430.0}, {860.0, 100.0}
    });
    shark_trails.push_back({
        {960.0, 100.0}, {960.0, 430.0}, {1290.0, 430.0}, {1290.0, 100.0}
    });
    shark_trails.push_back({
        {1390.0, 100.0}, {1640.0, 100.0}, {1640.0, 430.0}, {1390.0, 430.0}
    });

    shark_trails.push_back({
        {150.0, 500.0}, {430.0, 500.0}, {430.0, 900.0}, {150.0, 900.0}
    });
    shark_trails.push_back({
        {530.0, 500.0}, {530.0, 900.0}, {860.0, 900.0}, {860.0, 500.0}
    });
    shark_trails.push_back({
        {960.0, 500.0}, {960.0, 900.0}, {1290.0, 900.0}, {1290.0, 500.0}
    });
    shark_trails.push_back({
        {1390.0, 500.0}, {1640.0, 500.0}, {1640.0, 900.0}, {1390.0, 900.0}
    });
}

void SharkGame::handle_up(SDL_Keycode key, Uint8 mouse) {
    if (state != PLAYING) return;
    for (auto& ship : ships) {
        ship.handle_up(key, mouse, fruits_in_air);
    }
}

void SharkGame::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (state != PLAYING) return;
    for (auto& ship : ships) {
        ship.handle_down(key, mouse);
    }
}


