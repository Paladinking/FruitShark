#ifndef FRUITSHARK_SHARKGAME_H
#define FRUITSHARK_SHARKGAME_H
#include <vector>
#include "engine/game.h"
#include "engine/input.h"
#include "ship.h"
#include "config.h"
#include "shark.h"
#include "fruit.h"

class SharkGame : public State {
public:
    SharkGame() : State() {}

    void tick(Uint64 delta, StateStatus& res) override;

    void init(WindowState* window_state) override;

    void render() override;

    void handle_up(SDL_Keycode key, Uint8 mouse) override;

    void handle_down(SDL_Keycode key, Uint8 mouse) override;
private:
    std::unique_ptr<HoldInput> exit_input;
    std::vector<Ship> ships;
    std::vector<Shark> sharks;
    std::vector<Fruit> fruitsInAir;
    std::vector<Fruit> fruitsInWater;

    std::vector<std::vector<Vector2D>> shark_trails;

    void create_shark_trails();

    SDL_Rect game_viewport = {UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT};
    SDL_Rect ui_viewport = {0, 0, LOGICAL_WIDTH, LOGICAL_HEIGHT};
};
#endif //FRUITSHARK_SHARKGAME_H
