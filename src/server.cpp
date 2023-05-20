#include "server.h"

void Server::init(WindowState *window_state) {
    State::init(window_state);
    initialize();
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        inputs.push_back(new bool[4]);
        inputs[i][0] = false;
        inputs[i][1] = false;
        inputs[i][2] = false;
        inputs[i][3] = false;
    }
}

void Server::tick(Uint64 delta, StateStatus &res) {
    double dDelta = static_cast<double>(delta) / 1000.0;

    tick_physics(dDelta, inputs);
}

void Server::cannon_fired(Vector2D position, Vector2D velocity, FruitType type) {
    fruits_in_air.emplace_back(position, velocity, type);
}

void Server::create_bite(Vector2D position) {

}

void Server::ship_destroyed(int id) {

}

void Server::fruit_hit_water(const Fruit &fruit) {

}

void Server::fruit_hit_player(const Fruit &fruit, int player_id) {

}

void Server::pickup_created(const Pickup &pickup) {

}
