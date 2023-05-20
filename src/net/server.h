#ifndef FRUITSHARK_SERVER_H
#define FRUITSHARK_SERVER_H
#include "engine/game.h"
#include <vector>
#include <future>
#include "entities/ship.h"
#include "entities/shark.h"
#include "entities/bite.h"
#include "entities/pickup.h"
#include "net/net.h"



class Server : public Game, public GameState {
public:
    Server();

    void tick(Uint64 delta) override;

    void init() override;

    void cannon_fired(Vector2D position, Vector2D velocity, FruitType type) override;

    void ship_hurt(Vector2D position, int player_id, int dmg) override;

    void ship_destroyed(int id) override;

    void fruit_hit_water(int fruit, Vector2D position) override;

    void fruit_hit_player(int fruit, int player_id) override;

    void pickup_created(int x, int y, FruitType type) override;

private:
    std::vector<bool*> inputs;
    std::future<bool> message_future;

    std::vector<Uint8> buffer;

    std::unique_ptr<ENetHost, HostDeleter> server;
};


#endif //FRUITSHARK_SERVER_H
