#include "server.h"

void Server::init(WindowState *window_state) {
    State::init(window_state);
}

void Server::tick(Uint64 delta, StateStatus &res) {
    double dDelta = static_cast<double>(delta) / 1000.0;
    for (int i = 0; i < ships.size(); ++i) {
        // TODO handle input
        bool input[4] = {false, false, false, false};
        ships[i].tick_physics(dDelta, input, fruits_in_air);
    }
    for (auto& shark : sharks) {
        shark.tick_physics(dDelta, get_shark_trails(), fruits_in_water, ships);
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
                    Vector2D pos = shark.get_mouth();
                    // TODO Create bite
                    if (ships[i].is_dead()) {
                        ships.erase(ships.begin() + i);
                        --i;
                        if (ships.size() == 1) {
                            // TODO Game over
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
        fruit.tick_physics(dDelta, to_be_added);
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

        if (fruit.in_water) {
            // TODO Send fruit landed in water
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
        fruit.tick_physics(dDelta, to_be_added);
        for (auto& shark : sharks) {
            if (shark.intersects(fruit.get_position(), fruit.get_radius())) {
                fruit.eaten = true;
                break;
            }
        }
        if (fruit.eaten) {
            // TODO send fruit eaten
            fruits_in_water[i] = fruits_in_water[fruits_in_water.size() - 1];
            fruits_in_water.pop_back();
            --i;
        }
    }

    for (auto& pickup : pickups) {
        pickup.tick_physics(dDelta);
    }

    pickup_delay -= dDelta;
    if(pickup_delay < 0.0) {
        pickup_delay = PICKUP_SPAWN_TIME;
        Pickup::create(pickups);
    }
}
