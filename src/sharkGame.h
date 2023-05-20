#ifndef FRUITSHARK_SHARKGAME_H
#define FRUITSHARK_SHARKGAME_H
#include <vector>
#include "engine/game.h"
#include "engine/input.h"
#include "engine/ui.h"
#include "entities/ship.h"
#include "config.h"
#include "entities/shark.h"
#include "entities/bite.h"
#include "entities/fruit.h"
#include "entities/pickup.h"
#include "gameState.h"

class Restarter : public State {
    void tick(Uint64 delt , StateStatus& res) override;
};

class Input {
public:
    explicit Input(const char *const *bindings);

    void update(const Uint8 *keyboard, Uint32 mouse_mask);

    std::unique_ptr<HoldInput> left, right, forwards;
    std::unique_ptr<PressInput> left_cannon, right_cannon;

    bool hold_state[4] = {false, false, false, false};
};

class SharkGame : public State, public GameState {
public:
    SharkGame() : State() {}

    void tick(Uint64 delta, StateStatus& res) override;

    void init(WindowState* window_state) override;

    void render() override;

    void handle_up(SDL_Keycode key, Uint8 mouse) override;

    void handle_down(SDL_Keycode key, Uint8 mouse) override;

    void fruit_fired(Vector2D position, Vector2D velocity, FruitType type, bool cannon) override;

    void ship_hurt(Vector2D position, int player_id, int dmg) override;

    void ship_destroyed(int id) override;

    void fruit_hit_water(int fruit, Vector2D position) override;

    void fruit_hit_player(int fruit, int player_id) override;

    void pickup_created(int x, int y, FruitType type) override;

    void fruit_eaten(int id) override;

    void pickup_taken(int id) override;
private:
    std::unique_ptr<HoldInput> exit_input, restart_input;
    std::vector<Input> inputs;
    std::vector<bool*> input_values;
    std::vector<Bite> bites;

    SDL_Rect game_viewport = {UI_SIZE, 0, GAME_WIDTH, GAME_HEIGHT};

    enum {
        STARTUP, PLAYING, GAME_OVER
    } state = STARTUP;

    double startup_delay = 3.999;

    TextBox startup_textures[4];

    TextBox game_over[3];
};
#endif //FRUITSHARK_SHARKGAME_H
