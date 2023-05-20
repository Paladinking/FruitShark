#ifndef FRUITSHARK_FRUIT_H
#define FRUITSHARK_FRUIT_H

#include "config.h"
#include "vector2D.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "asset_handlers/textureHandler.h"
#include "entities/entity.h"
#include "gameState.h"
#include <vector>

enum class FruitType : Uint8 {
    APPLE = 1,
    BANANA = 2,
    STARFRUIT = 3,
    POMEGRANATE = 4,
    POMEGRANATE_SEED = 5
};

const Texture* get_fruit_texture(FruitType type);

class Fruit : public Entity {
public:
    Fruit(Vector2D position, Vector2D velocity, FruitType type);

    void render() const;

    void tick_physics(double delta, GameState& game_state);

    void land(GameState& game_state);

    [[nodiscard]] const Vector2D &get_position() const;

    void set_position(Vector2D pos);

    bool in_water = false;
    bool eaten = false;

    [[nodiscard]] double get_radius() const;

    double get_duration();

private:
    const Texture* texture;

    FruitType type;

    Vector2D velocity;
    Vector2D acceleration;

    double time_in_air = 0.0;
    double max_time_in_air;
    double max_velocity;
};

#endif //FRUITSHARK_FRUIT_H