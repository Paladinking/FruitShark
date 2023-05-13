#include "shark.h"
#include "engine/engine.h"

constexpr double SHARK_ACCELERATION = 900.0;

Shark::Shark(const double x, const double y) : Entity(x, y, 20, 20) {
    texture.load_from_file("Shark.png", 20, 20);
}

void Shark::tick(const double delta, const std::vector<std::vector<Vector2D>>& trails) {
    if (trail != nullptr) {
        Vector2D target = trail->at(trail_index);
        if (target.distance_squared(position) < 100.0) {
            if (engine::random(0, 100) > 80) {
                trail = &trails[engine::random(0, static_cast<int>(trails.size()))];
                trail_index = engine::random(0, static_cast<int>(trail->size()));
            } else {
                trail_index = (trail_index + 1) % trail->size();
                target = trail->at(trail_index);
            }
        }
        target.subtract(position);
        target.normalize();
        angle = target.get_angle();
        acceleration.add_scaled(target, SHARK_ACCELERATION / 5.0);
    }
    Entity::move(delta);
}

void Shark::render() const {
    texture.render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

void Shark::set_trail(const std::vector<Vector2D> *new_trail) {
    trail = new_trail;
}
