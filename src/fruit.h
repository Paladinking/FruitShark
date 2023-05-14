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

    [[nodiscard]] const Vector2D &getPosition() const;

    bool inWater = false;
    bool eaten = false;

    [[nodiscard]] double getRadius() const;

private:
     const Texture* texture1;
    const Texture* texture2;

    FruitType type;

    Vector2D position;
    Vector2D velocity;

    double angle = 0.0;
    double timeAlive = 0.0;
};

#endif //FRUITSHARK_FRUIT_H