#include "shark.h"

constexpr double SHARK_ACCELERATION = 900.0;
constexpr double FRUIT_DETECTION_RANGE = 300.0;
constexpr double PLAYER_DETECTION_RANGE = 180.0;
constexpr double BITE_DELAY = 1.0;

Shark Shark::create_shark(Type type, const double x, const double y) {
    switch (type) {
        case Type::GREAT_WHITE:
            return {x, y, 60, 120, 15, 2.0, 0.9, TextureID::WHITE_SHARK};
        case Type::TIGER: default:
            return {x, y, 40, 90, 10, 1.0, 1.0, TextureID::SHARK};
    }
}

Shark::Shark(const double x, const double y, int w, int len, int dmg, double range_factor, double acc_factor, TextureID id) :
        BoxEntity(x, y, w, len), dmg(dmg), range_factor(range_factor), acc_factor(acc_factor),
        texture(texture_handler.get_textures(id))
{
    animation_stage = (engine::random(0, 4) / 6.0);
}

void Shark::tick_animation(const double delta) {
    animation_stage += delta;
    if (animation_stage >= 1.3) animation_stage = 0.0;
}

void Shark::tick_physics(const double delta,
                 const std::vector<std::vector<Vector2D>>& trails,
                 const std::vector<Fruit>& fruitsInWater,
                 const std::vector<Ship>& ships) {
    Vector2D target = Vector2D(position.x + FRUIT_DETECTION_RANGE * range_factor, position.y);
    bool fruit_target = false;
    double acc = SHARK_ACCELERATION;

    for (auto& ship : ships) {
        if (ship.has_fruit_smell() &&
            ship.get_position().distance_squared(position) <
            target.distance_squared(position))
        {
            target = ship.get_position();
            fruit_target = true;
        }
    }

    if (!fruit_target) {
        for (auto& fruit : fruitsInWater) {
            Vector2D fruitPosition = fruit.get_position();
            if (position.distance_squared(fruitPosition) < position.distance_squared(target)) {
                target = fruitPosition;
                fruit_target = true;
            }
        }
    }
    if (!fruit_target) {
        target = Vector2D(position.x + PLAYER_DETECTION_RANGE * range_factor, position.y);
        for (auto& ship : ships) {
            if (ship.get_position().distance_squared(position) < target.distance_squared(position))
            {
                target = ship.get_position();
                fruit_target = true;
                acc = SHARK_ACCELERATION / 3.0;
            }
        }
    }
    if (!fruit_target) {
        acc = SHARK_ACCELERATION / 5.0;
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
    } else {
        trail = nullptr;
    }
    target.subtract(position);
    target.normalize();
    angle = target.get_angle();
    if (bite_delay > 0) {
        bite_delay -= delta;
    } else {
        acceleration.add_scaled(target, acc * acc_factor);
    }
    BoxEntity::move(delta);
}

void Shark::render() const {
    const Texture* textures[] = {texture, texture + 1, texture + 2, texture + 1};
    (animation_stage < 0.665) ?
    textures[static_cast<int>(animation_stage * 6)]->
    render(static_cast<int>(position.x), static_cast<int>(position.y), angle) :
    textures[static_cast<int>((animation_stage - 0.665) * 6)]->
    render(static_cast<int>(position.x), static_cast<int>(position.y),
             angle + 3.1415, SDL_FLIP_HORIZONTAL);
}

void Shark::set_trail(const std::vector<Vector2D> *new_trail) {
    trail = new_trail;
}

bool Shark::bite(Ship &ship) {
    if (bite_delay <= 0) {
        ship.get_bitten(dmg);
        bite_delay = BITE_DELAY;
        return true;
    }
    return false;
}

Vector2D Shark::get_mouth() const {
    return {(corners[1].x + corners[2].x) / 2, (corners[1].y + corners[2].y) / 2};
}
