#include "shark.h"

constexpr double SHARK_ACCELERATION = 900.0;

Shark::Shark(const double x, const double y) :
Entity(x, y, 20, 20),
texture1(&textureHandler.getTexture(TextureID::SHARK1)),
texture2(&textureHandler.getTexture(TextureID::SHARK2)),
texture3(&textureHandler.getTexture(TextureID::SHARK3)) {
    animationStage = engine::random(0, 80);
}

void Shark::tick(const double delta, const std::vector<std::vector<Vector2D>>& trails) {
    ++animationStage;
    if (animationStage > 80) animationStage = 0;
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
    switch (animationStage) {
        case 0 ... 10:
        case 41 ... 50:
            texture1->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            break;
        case 11 ... 20:
        case 31 ... 40:
            texture2->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            break;
        case 21 ... 30:
            texture3->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            break;
        case 51 ... 60:
        case 71 ... 80:
            texture2->render(static_cast<int>(position.x), static_cast<int>(position.y),
                             angle + 3.1415, SDL_FLIP_HORIZONTAL);
            break;
        case 61 ... 70:
            texture3->render(static_cast<int>(position.x), static_cast<int>(position.y),
                             angle + 3.1415, SDL_FLIP_HORIZONTAL);
            break;
    }
}

void Shark::set_trail(const std::vector<Vector2D> *new_trail) {
    trail = new_trail;
}
