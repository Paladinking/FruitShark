#ifndef FRUITSHARK_SHARKGAME_H
#define FRUITSHARK_SHARKGAME_H
#include "engine/game.h"
#include "engine/input.h"
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
};
#endif //FRUITSHARK_SHARKGAME_H
