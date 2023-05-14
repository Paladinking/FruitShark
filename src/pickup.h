#ifndef FRUITSHARK_PICKUP_H
#define FRUITSHARK_PICKUP_H

#include "vector2D.h"
#include "fruit.h"
#include "config.h"
#include "engine/engine.h"

class Pickup {
public:
    Pickup(int x, int y, FruitType type);

    void tick(double delta);

    void render() const;

    FruitType getType();

    const Vector2D& get_position() const;

    double get_radius() const;

    double timeAlive;

private:
    const Texture* boxTexture;
    const Texture* fruitTexture;

    Vector2D position;

    double angle;
    int diameter;
    FruitType type;
};

#endif //FRUITSHARK_PICKUP_H