#ifndef FRUITSHARK_SERVER_H
#define FRUITSHARK_SERVER_H
#include "engine/game.h"
#include <vector>
#include "ship.h"
#include "shark.h"
#include "bite.h"
#include "pickup.h"

class Server : public State, public GameState {
public:
    void tick(Uint64 delta, StateStatus& res) override;

    void init(WindowState* window_state) override;

    void cannon_fired(Vector2D position, Vector2D velocity, FruitType type) override;

    void create_bite(Vector2D position) override;

    void ship_destroyed(int id) override;

    void fruit_hit_water(const Fruit &fruit) override;

    void fruit_hit_player(const Fruit &fruit, int player_id) override;

    void pickup_created(const Pickup &pickup) override;

private:
    std::vector<bool*> inputs;
};


#endif //FRUITSHARK_SERVER_H
