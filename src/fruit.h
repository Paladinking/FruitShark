#ifndef FRUITSHARK_FRUIT_H
#define FRUITSHARK_FRUIT_H

#include "vector2D.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "textureHandler.h"
#include "config.h"

enum class FruitType {
    APPLE = 1,
    BANANA = 2,
    STARFRUIT = 3
};

class Fruit {
public:
    Fruit(Vector2D position, Vector2D velocity, FruitType type);

    void render() const;

    void tick(double delta);

    void land();

    [[nodiscard]] const Vector2D &get_position() const;

    bool inWater = false;
    bool eaten = false;

    [[nodiscard]] double get_radius() const;

private:
    const Texture* texture;

    FruitType type;

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    double angle = 0.0;
    double timeInAir = 0.0;
    double maxTimeInAir;
    double maxVelocity;
};

#endif //FRUITSHARK_FRUIT_H