#include "Ship.h"
#include "sound.h"
#include "shark.h"

constexpr double ACCELERATION = 800.0;
constexpr double MIN_POWER = 750.0;
constexpr double MAX_POWER = 1250.0;
constexpr double POWER_PER_SECOND = 1000.0;
constexpr double COOLDOWN_TIME = 0.5;


Ship::Ship(double x, double y, const char *const *bindings, const TextureID sail_color, int id, double angle) :
Entity(x, y, 25, 50, angle),
shipTexture(&textureHandler.getTexture(TextureID::SHIP)),
mastsTexture(&textureHandler.getTexture(TextureID::MASTS)),
sailsTexture(&textureHandler.getTexture(sail_color)),
id(id),
leftCannon(),
rightCannon() {
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

    if (leftCannon.cooldown > 0.0) leftCannon.cooldown -= delta;
    if (leftCannon.cooldown < 0.0) leftCannon.cooldown = 0.0;
    if (leftCannon.cooldown == 0.0 && isChargingLeft) {
        leftCannon.power += POWER_PER_SECOND * delta;
        if (leftCannon.power > MAX_POWER) {
            leftCannon.power = MAX_POWER;
            fireLeftCannon(fruits);
        }
    }
    if (rightCannon.cooldown > 0.0) rightCannon.cooldown -= delta;
    if (rightCannon.cooldown < 0.0) rightCannon.cooldown = 0.0;
    if (rightCannon.cooldown == 0.0 && isChargingRight) {
        rightCannon.power += POWER_PER_SECOND * delta;
        if (rightCannon.power > MAX_POWER) {
            rightCannon.power = MAX_POWER;
            fireRightCannon(fruits);
        }
    }
    if (smell_duration > 0.0) {
        smell_duration -= delta;
    }
    Entity::move(delta);
}

void Ship::render() const {
    Vector2D cannonPosition = leftCannonPosition();
    leftCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);
    cannonPosition = rightCannonPosition();
    rightCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);

    shipTexture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    sailsTexture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    mastsTexture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);

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

Vector2D Ship::leftCannonPosition() const {
    return {position.x - (width * cos(angle + PI / 2.0)) / 1.5,
                    position.y - (width * sin(angle + PI / 2.0)) / 1.5};
}

Vector2D Ship::rightCannonPosition() const {
    return {position.x + (width * cos(angle + PI / 2.0)) / 1.5,
            position.y + (width * sin(angle + PI / 2.0)) / 1.5};
}

void Ship::fireLeftCannon(std::vector<Fruit> &fruits) {
    sound::play(sound::Id::CANNON);
    Vector2D fruitPosition = leftCannonPosition();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle - PI / 2) * leftCannon.power;
    fruitVelocity.y += sin(angle - PI / 2) * leftCannon.power;
    fruits.emplace_back(fruitPosition, fruitVelocity, FruitType::APPLE);
    leftCannon.power = 0.0;
    leftCannon.cooldown = COOLDOWN_TIME;
    isChargingLeft = false;
}

void Ship::fireRightCannon(std::vector<Fruit> &fruits) {
    sound::play(sound::Id::CANNON);
    Vector2D fruitPosition = rightCannonPosition();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle + PI / 2) * rightCannon.power;
    fruitVelocity.y += sin(angle + PI / 2) * rightCannon.power;
    fruits.emplace_back(fruitPosition, fruitVelocity, FruitType::POMEGRANATE);
    rightCannon.power = 0.0;
    rightCannon.cooldown = COOLDOWN_TIME;
    isChargingRight = false;
}

void Ship::handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits) {
    if (fire_left->is_targeted(key, mouse)) {
        if (isChargingLeft && leftCannon.cooldown == 0.0) {
            fireLeftCannon(fruits);
            leftCannon.power = 0.0;
        } else {
            isChargingLeft = false;
        }
    }
    if (fire_right->is_targeted(key, mouse)) {
        if (isChargingRight && rightCannon.cooldown == 0.0) {
            fireRightCannon(fruits);
            rightCannon.power = 0.0;
        } else {
            isChargingRight = false;
        }
    }
}

void Ship::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (fire_left->is_targeted(key, mouse) && !isChargingLeft) {
        leftCannon.power = MIN_POWER;
        isChargingLeft = true;
    }
    if (fire_right->is_targeted(key, mouse) && !isChargingRight) {
        rightCannon.power = MIN_POWER;
        isChargingRight = true;
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


Cannon::Cannon() : texture(&textureHandler.getTexture(TextureID::CANNON)) {}

void Cannon::render(const int x, const int y, const double angle) const {
    texture->render(x, y, angle + PI / 2);
}


