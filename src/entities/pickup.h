#ifndef FRUITSHARK_PICKUP_H
#define FRUITSHARK_PICKUP_H

#include "vector2D.h"
#include "config.h"
#include "entities/fruit.h"
#include "engine/engine.h"

class Pickup : public Entity{
public:
    Pickup(double x, double y, FruitType type);

    void tick_animation(double delta);

    void render() const;

    FruitType get_type();

    [[nodiscard]] const Vector2D& get_position() const;

    [[nodiscard]] double get_radius() const;

private:
    const Texture* box_texture;
    const Texture* fruit_texture;

    FruitType type;
};

#endif //FRUITSHARK_PICKUP_H