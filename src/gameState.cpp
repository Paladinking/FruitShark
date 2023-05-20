#include "gameState.h"
#include "entities/shark.h"

void GameState::initialize() {
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        ships.emplace_back(START_X[PLAYER_COUNT - 1][i], START_Y[PLAYER_COUNT - 1][i],
                           COLORS[i], i, PI * i);
    }
    const std::vector<std::vector<Vector2D>>& shark_trails = get_shark_trails();
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
        if (type > 80) {
            sharks.push_back(Shark::create_shark(Shark::Type::GREAT_WHITE, x, y));
        } else {
            sharks.push_back(Shark::create_shark(Shark::Type::TIGER, x, y));
        }
        sharks[i].set_trail(&shark_trails[path]);
    }
    pickup_delay = PICKUP_SPAWN_TIME;
}

void GameState::tick_physics(double delta, const std::vector<bool*>& inputs) {
    for (int i = 0; i < ships.size(); ++i) {
        const bool* input = inputs[i];
        ships[i].tick_physics(delta, input, *this);
    }
    for (auto& shark : sharks) {
        shark.tick_physics(delta, get_shark_trails(), fruits_in_water, ships);
    }

    for (int i = 0; i < ships.size(); ++i) {
        for (int j = i + 1; j < ships.size(); ++j) {
            if (ships[i].intersects(ships[j])) {
                ships[i].handle_Collision(ships[j]);
            }
        }
        for (int j = 0; j < pickups.size(); ++j) {
            if (ships[i].intersects(pickups[j].get_position(), pickups[j].get_radius())) {
                ships[i].add_fruits(pickups[j].get_type(), 4);
                pickups.erase(pickups.begin() + j);
                --j;
            }
        }
    }

    for (auto &shark : sharks) {
        for (int i = 0; i < ships.size(); ++i) {
            if (ships[i].intersects(shark)) {
                if (shark.bite(ships[i])) {
                    this->create_bite(shark.get_mouth());
                    if (ships[i].is_dead()) {
                        ships.erase(ships.begin() + i);
                        --i;
                        this->ship_destroyed(ships[i].id);
                    }
                }
                ships[i].handle_Collision(shark);
            }
        }
    }

    std::vector<Fruit> to_be_added;
    for (int i = 0; i < fruits_in_air.size(); ++i) {
        auto& fruit = fruits_in_air[i];
        fruit.tick_physics(delta, to_be_added);
        bool collision = false;
        for (auto& ship : ships) {
            if (ship.intersects(fruit.get_position(), fruit.get_radius())) {
                this->fruit_hit_player(fruit, ship.id);
                ship.add_fruit_smell(fruit.get_duration());
                fruits_in_air[i] = fruits_in_air[fruits_in_air.size() - 1];
                fruits_in_air.pop_back();
                --i;
                collision = true;
                break;
            }
        }
        if (collision) continue;

        if (fruit.in_water) {
            this->fruit_hit_water(fruit);
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
        fruit.tick_physics(delta, to_be_added);
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
        pickup.tick_physics(delta);
    }

    pickup_delay -= delta;
    if(pickup_delay < 0.0) {
        pickup_delay = PICKUP_SPAWN_TIME;
        Pickup::create(pickups);
        this->pickup_created(pickups[pickups.size() - 1]);
    }
}
