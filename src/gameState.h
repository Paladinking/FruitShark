#ifndef FRUITSHARK_GAMESTATE_H
#define FRUITSHARK_GAMESTATE_H
#include "fruit.h"
#include "pickup.h"
#include "config.h"

class GameState {
public:
    void tick_physics(double delta, const std::vector<bool*> inputs);

    void initialize();

    virtual void cannon_fired(Vector2D position, Vector2D velocity, FruitType type) = 0;

    virtual void create_bite(Vector2D position) = 0;

    virtual void ship_destroyed(int id) = 0;

    virtual void fruit_hit_water(const Fruit &fruit) = 0;

    virtual void fruit_hit_player(const Fruit &fruit, int player_id) = 0;

    virtual void pickup_created(const Pickup& pickup) = 0;
protected:
    std::vector<Ship> ships;
    std::vector<Shark> sharks;
    std::vector<Fruit> fruits_in_air;
    std::vector<Fruit> fruits_in_water;
    std::vector<Pickup> pickups;

    double pickup_delay = PICKUP_SPAWN_TIME;
};




#endif //FRUITSHARK_GAMESTATE_H
