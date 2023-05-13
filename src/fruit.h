#ifndef FRUITSHARK_FRUIT_H
#define FRUITSHARK_FRUIT_H

#include "vector2D.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "textureHandler.h"
#include "config.h"

enum class FruitType {
    APPLE = 3,
    BANANA = 4,
    STARFRUIT = 5
};

class Fruit {
public:
    Fruit(Vector2D position, Vector2D velocity, FruitType type);

    void render() const;

    void tick(double delta);

    void kill();

    bool dead = false;

private:
    const Texture* texture;

    FruitType type;

    Vector2D position;
    Vector2D velocity;

    double angle = 0.0;
    double timeAlive = 0.0;
};

#endif //FRUITSHARK_FRUIT_H