#ifndef FRUITSHARK_CLIENT_H
#define FRUITSHARK_CLIENT_H
#include <enet/enet.h>
#include "engine/game.h"
#include "engine/input.h"
#include "gameState.h"
#include "net.h"

class Client : public State, public GameState {
public:
    Client();

    void init(WindowState *window_state) override;

    void tick(Uint64 delta, StateStatus &res) override;

    void render() override;

    void handle_down(SDL_Keycode key, Uint8 mouse) override;

    void handle_up(SDL_Keycode key, Uint8 mouse) override;


    void cannon_fired(Vector2D position, Vector2D velocity, FruitType type) override;
    void ship_destroyed(int id) override;
    void fruit_hit_water(int fruit, Vector2D position) override;
    void fruit_hit_player(int fruit, int player_id) override;
    void pickup_created(int x, int y, FruitType type) override;
    void ship_hurt(Vector2D position, int player_id, int dmg) override;

private:
    void handle_event(Uint8 event, const Uint8* buffer);

    void load_state(const Uint8* buffer);

    std::unique_ptr<PressInput> forward, left, right, left_cannon, right_cannon;

    std::unique_ptr<ENetHost, HostDeleter> client;
    ENetPeer* peer = nullptr;

    SDL_Rect game_viewport = {UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT};

    Uint8 id = 255;
};


#endif //FRUITSHARK_CLIENT_H
