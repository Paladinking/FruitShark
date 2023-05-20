#ifndef FRUITSHARK_SERVER_H
#define FRUITSHARK_SERVER_H
#include "engine/game.h"
#include <vector>
#include "ship.h"
#include "shark.h"
#include "bite.h"
#include "pickup.h"

class Server : State {
    void tick(Uint64 delta, StateStatus& res) override;

    void init(WindowState* window_state) override;

private:
    std::vector<Ship> ships;
    std::vector<Shark> sharks;
    std::vector<Fruit> fruits_in_air;
    std::vector<Fruit> fruits_in_water;
    std::vector<Pickup> pickups;

    std::vector<std::vector<Vector2D>> shark_trails;

    double pickup_delay = PICKUP_SPAWN_TIME;
};


#endif //FRUITSHARK_SERVER_H
