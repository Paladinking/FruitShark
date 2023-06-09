#include "Ship.h"
#include "sound.h"
#include "shark.h"


Ship::Ship(double x, double y, const char *const *bindings, const TextureID sail_color, int id, double angle) :
        Entity(x, y, 25, 50, angle),
        ship_texture(&texture_handler.get_texture(TextureID::SHIP)),
        masts_texture(&texture_handler.get_texture(TextureID::MASTS)),
        sails_texture(&texture_handler.get_texture(sail_color)),
        id(id),
        left_cannon(),
        right_cannon() {
    forward = get_hold_input(bindings[0]);
    left = get_hold_input(bindings[1]);
    back = get_hold_input(bindings[2]);
    right = get_hold_input(bindings[3]);

    fire_left = get_press_input(bindings[4]);
    fire_right = get_press_input(bindings[5]);
}

void Ship::tick(double delta, const Uint8 *keyboard, Uint32 mouse_mask, std::vector<Fruit>& fruits) {
    Vector2D vel_delta = { cos(angle), sin(angle)};
    if (forward->is_pressed(keyboard, mouse_mask)) {
        acceleration.add_scaled(vel_delta,  ACCELERATION);
    }

    if (left->is_pressed(keyboard, mouse_mask)) {
        angle -= 120.0 * delta * PI / 180.0; // Remove to stop rotate while still
        velocity.rotate(-120.0 * delta * PI / 180.0);
    }
    if (right->is_pressed(keyboard, mouse_mask)) {
        angle += 120.0 * delta * PI / 180.0; // Remove to stop rotate while still
        velocity.rotate(120.0 * delta * PI / 180.0);
    }

    if (left_cannon.cooldown > 0.0) left_cannon.cooldown -= delta;
    if (left_cannon.cooldown < 0.0) left_cannon.cooldown = 0.0;
    if (left_cannon.cooldown == 0.0 && is_charging_left) {
        left_cannon.power += POWER_PER_SECOND * delta;
        if (left_cannon.power > MAX_POWER) {
            left_cannon.power = MAX_POWER;
            fire_left_cannon(fruits);
        }
    }
    if (right_cannon.cooldown > 0.0) right_cannon.cooldown -= delta;
    if (right_cannon.cooldown < 0.0) right_cannon.cooldown = 0.0;
    if (right_cannon.cooldown == 0.0 && is_charging_right) {
        right_cannon.power += POWER_PER_SECOND * delta;
        if (right_cannon.power > MAX_POWER) {
            right_cannon.power = MAX_POWER;
            fire_right_cannon(fruits);
        }
    }
    if (smell_duration > 0.0) {
        smell_duration -= delta;
    }
    Entity::move(delta);
}

void Ship::render() const {
    Vector2D cannonPosition = left_cannon_position();
    left_cannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);
    cannonPosition = right_cannon_position();
    right_cannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);

    ship_texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    sails_texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    masts_texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect bar = {static_cast<int>(position.x - length / 2.0),
                    static_cast<int>(position.y + width / 2.0 + 15.0),
                    length, width / 4
                };
    SDL_RenderFillRect(gRenderer, &bar);
    bar.w = (length * std::max(hp, 0)) / MAX_HP;
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &bar);

}

Vector2D Ship::left_cannon_position() const {
    return {position.x - (width * cos(angle + PI / 2.0)) / 1.5,
                    position.y - (width * sin(angle + PI / 2.0)) / 1.5};
}

Vector2D Ship::right_cannon_position() const {
    return {position.x + (width * cos(angle + PI / 2.0)) / 1.5,
            position.y + (width * sin(angle + PI / 2.0)) / 1.5};
}

void Ship::fire_left_cannon(std::vector<Fruit> &fruits) {
    sound::play(sound::Id::CANNON);
    Vector2D fruitPosition = left_cannon_position();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle - PI / 2) * left_cannon.power;
    fruitVelocity.y += sin(angle - PI / 2) * left_cannon.power;
    --fruit_count;
    if (fruit_count  < 0) {
        fruit_type = FruitType::APPLE;
    }
    fruits.emplace_back(fruitPosition, fruitVelocity, fruit_type);
    left_cannon.power = 0.0;
    left_cannon.cooldown = COOLDOWN_TIME;
    is_charging_left = false;
}

void Ship::fire_right_cannon(std::vector<Fruit> &fruits) {
    sound::play(sound::Id::CANNON);
    Vector2D fruitPosition = right_cannon_position();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle + PI / 2) * right_cannon.power;
    fruitVelocity.y += sin(angle + PI / 2) * right_cannon.power;
    fruits.emplace_back(fruitPosition, fruitVelocity, fruit_type);
    right_cannon.power = 0.0;
    right_cannon.cooldown = COOLDOWN_TIME;
    is_charging_right = false;
}

void Ship::handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits) {
    if (fire_left->is_targeted(key, mouse)) {
        if (is_charging_left && left_cannon.cooldown == 0.0) {
            fire_left_cannon(fruits);
            left_cannon.power = 0.0;
        } else {
            is_charging_left = false;
        }
    }
    if (fire_right->is_targeted(key, mouse)) {
        if (is_charging_right && right_cannon.cooldown == 0.0) {
            fire_right_cannon(fruits);
            right_cannon.power = 0.0;
        } else {
            is_charging_right = false;
        }
    }
}

void Ship::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (fire_left->is_targeted(key, mouse) && !is_charging_left) {
        left_cannon.power = MIN_POWER;
        is_charging_left = true;
    }
    if (fire_right->is_targeted(key, mouse) && !is_charging_right) {
        right_cannon.power = MIN_POWER;
        is_charging_right = true;
    }
}

void Ship::get_bitten(int damage) {
    hp -= damage;
}

bool Ship::has_fruit_smell() const {
    return smell_duration > 0.0;
}

void Ship::add_fruit_smell(double duration) {
    smell_duration += duration;
}

bool Ship::is_dead() const {
    return hp <= 0;
}

void Ship::add_fruits(FruitType type, int count) {
    if (type == fruit_type) {
        fruit_count += count;
    } else {
        fruit_type = type;
        fruit_count = count;
    }
}


Cannon::Cannon() : texture(&texture_handler.get_texture(TextureID::CANNON)) {}

void Cannon::render(const int x, const int y, const double angle) const {
    texture->render(x, y, angle + PI / 2);
}


