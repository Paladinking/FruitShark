#ifndef FRUITSHARK_ENTITY_H
#define FRUITSHARK_ENTITY_H
#include "vector2D.h"
#include <SDL.h>

constexpr double PI = 3.14159265;

class Entity {
public:
    Entity(double x, double y, int width, int length);

    [[nodiscard]] bool intersects(const Entity& other) const;
protected:
    Vector2D position, velocity {}, acceleration {};

    double angle = 0.0;

    int width;
    int length;

    void move(double delta);

private:
    void update_bounds();

    void adjust_bounds(float dx, float dy);

    SDL_FPoint corners[4] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    SDL_FRect bounds = {0.0, 0.0, 0.0, 0.0};
};


#endif //FRUITSHARK_ENTITY_H
