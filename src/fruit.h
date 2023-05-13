#ifndef FRUITSHARK_FRUIT_H
#define FRUITSHARK_FRUIT_H

#include "vector2D.h"
#include "engine/texture.h"
#include "engine/engine.h"

enum FruitType {
    Apple,
    Banana,
    Starfruit
};

class Fruit {
public:
    Fruit(Vector2D position, Vector2D velocity, FruitType type);

    void render(int x, int y);

    void tick(double delta);

private:
    FruitType type;

    Vector2D position;
    Vector2D velocity;

    double angle = 0.0;
};

#endif //FRUITSHARK_FRUIT_H