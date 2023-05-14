#include "shark.h"
#include "sound.h"

constexpr double SHARK_ACCELERATION = 900.0;
constexpr double FRUIT_DETECTION_RANGE = 200.0;
constexpr double BITE_DELAY = 1.5;

Shark::Shark(const double x, const double y) :
Entity(x, y, 40, 90),
texture1(&textureHandler.getTexture(TextureID::SHARK1)),
texture2(&textureHandler.getTexture(TextureID::SHARK2)),
texture3(&textureHandler.getTexture(TextureID::SHARK3)) {
    animationStage = engine::random(0, 80);
}

void Shark::tick(const double delta,
                 const std::vector<std::vector<Vector2D>>& trails,
                 const std::vector<Fruit>& fruitsInWater) {
    ++animationStage;
    if (animationStage > 80) animationStage = 0;
    Vector2D target;
    bool noTarget = true;
    if (!fruitsInWater.empty()) {
        Vector2D closestFruitPosition = Vector2D(position.x + FRUIT_DETECTION_RANGE, position.y);
        for (auto& fruit : fruitsInWater) {
            Vector2D fruitPosition = fruit.getPosition();
            if (position.distance(fruitPosition) < position.distance(closestFruitPosition)) {
                closestFruitPosition = fruitPosition;
            }
        }
        if (!(closestFruitPosition.x == position.x + FRUIT_DETECTION_RANGE && closestFruitPosition.y == position.y)) {
            target = closestFruitPosition;
            noTarget = false;
            trail = nullptr;
        }
    }
    if (noTarget) {
        if (trail == nullptr) {
            trail = &trails[engine::random(0, static_cast<int>(trails.size()))];
        }
        target = trail->at(trail_index);
        if (target.distance_squared(position) < 100.0) {
            if (engine::random(0, 100) > 80) {
                trail = &trails[engine::random(0, static_cast<int>(trails.size()))];
                trail_index = engine::random(0, static_cast<int>(trail->size()));
            } else {
                trail_index = (trail_index + 1) % trail->size();
                target = trail->at(trail_index);
            }
        }
    }
    target.subtract(position);
    target.normalize();
    angle = target.get_angle();
    acceleration.add_scaled(target, SHARK_ACCELERATION / 5.0);
    if (bite_delay > 0) {
        bite_delay -= delta;
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
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff ,0xff);
    SDL_RenderDrawRectF(gRenderer, &bounds);
}

void Shark::set_trail(const std::vector<Vector2D> *new_trail) {
    trail = new_trail;
}

void Shark::bite(Ship &ship) {
    if (bite_delay <= 0) {
        sound::play(sound::Id::BITE);
        ship.get_bitten(10);
        bite_delay = BITE_DELAY;
    }
}
