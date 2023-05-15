#ifndef FRUITSHARK_SHARKGAME_H
#define FRUITSHARK_SHARKGAME_H
#include <vector>
#include "engine/game.h"
#include "engine/input.h"
#include "engine/ui.h"
#include "ship.h"
#include "config.h"
#include "shark.h"
#include "bite.h"
#include "fruit.h"
#include "pickup.h"

constexpr double PICKUP_SPAWN_TIME = 10.0;

class Restarter : public State {
    void tick(Uint64 delt , StateStatus& res) override;
};

class SharkGame : public State {
public:
    SharkGame() : State() {}

    void tick(Uint64 delta, StateStatus& res) override;

    void init(WindowState* window_state) override;

    void render() override;

    void handle_up(SDL_Keycode key, Uint8 mouse) override;

    void handle_down(SDL_Keycode key, Uint8 mouse) override;
private:
    std::unique_ptr<HoldInput> exit_input, restart_input;
    std::vector<Ship> ships;
    std::vector<Shark> sharks;
    std::vector<Bite> bites;
    std::vector<Fruit> fruits_in_air;
    std::vector<Fruit> fruits_in_water;
    std::vector<Pickup> pickups;

    std::vector<std::vector<Vector2D>> shark_trails;

    void create_shark_trails();

    void create_pickup();

    SDL_Rect game_viewport = {UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT};

    enum {
        STARTUP, PLAYING, GAME_OVER
    } state = STARTUP;

    double startup_delay = 3.999;
    double pickup_delay = PICKUP_SPAWN_TIME;

    TextBox startup_textures[4];

    TextBox game_over[3];
};
#endif //FRUITSHARK_SHARKGAME_H
