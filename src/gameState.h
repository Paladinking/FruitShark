#ifndef FRUITSHARK_GAMESTATE_H
#define FRUITSHARK_GAMESTATE_H
#include "config.h"

class GameState {
public:
    void tick_physics(double delta, const std::vector<bool*>& inputs);

    void initialize();

    virtual void fruit_fired(Vector2D position, Vector2D velocity, FruitType type, bool cannon) = 0;

    virtual void ship_hurt(Vector2D position, int player_id, int dmg) = 0;

    virtual void ship_destroyed(int id) = 0;

    virtual void fruit_hit_water(int fruit, Vector2D position) = 0;

    virtual void fruit_hit_player(int fruit, int player_id) = 0;

    virtual void pickup_created(int x, int y, FruitType type) = 0;

    virtual void fruit_eaten(int id) = 0;

    virtual void pickup_taken(int id) = 0;
protected:
    std::vector<Ship> ships;
    std::vector<Shark> sharks;
    std::vector<Fruit> fruits_in_air;
    std::vector<Fruit> fruits_in_water;
    std::vector<Pickup> pickups;

    std::vector<Fruit> to_be_added;

    double pickup_delay = PICKUP_SPAWN_TIME;
};




#endif //FRUITSHARK_GAMESTATE_H
