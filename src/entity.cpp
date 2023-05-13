#include "entity.h"
constexpr double FRICTION = 0.02;

Entity::Entity(double x, double y) : position(x, y) {}

void Entity::move(const double delta) {
    acceleration.x -= velocity.x * std::abs(velocity.x) * FRICTION;
    acceleration.y -= velocity.y * std::abs(velocity.y) * FRICTION;

    velocity.x += delta * acceleration.x;
    velocity.y += delta * acceleration.y;
    if (velocity.length_squared() == 0) return;
    angle = velocity.get_angle();
    Vector2D to_move = {velocity.x * delta, velocity.y * delta};
    position.add(to_move);
    acceleration.x = 0.0;
    acceleration.y = 0.0;
}