#include "Ship.h"

constexpr double ACCELERATION = 800.0;
constexpr double MIN_POWER = 200.0;
constexpr double MAX_POWER = 1000.0;
constexpr double POWER_PER_SECOND = 1000.0;


Ship::Ship(double x, double y, const char *const *bindings) :
Entity(x, y, 25, 50),
texture(&textureHandler.getTexture(TextureID::SHIP)),
leftCannon(width),
rightCannon(width) {
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

    if (isChargingLeft) {
        leftCannon.power += POWER_PER_SECOND * delta;
        if (leftCannon.power > MAX_POWER) {
            leftCannon.power = MAX_POWER;
            fireLeftCannon(fruits);
        }
    }
    if (isChargingRight) {
        rightCannon.power += POWER_PER_SECOND * delta;
        if (rightCannon.power > MAX_POWER) {
            rightCannon.power = MAX_POWER;
            fireRightCannon(fruits);
        }
    }

    Entity::move(delta);
}

void Ship::render() const {
    Vector2D cannonPosition = leftCannonPosition();
    leftCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);
    cannonPosition = rightCannonPosition();
    rightCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);

    texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
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
    Vector2D fruitPosition = leftCannonPosition();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle - PI / 2) * leftCannon.power;
    fruitVelocity.y += sin(angle - PI / 2) * leftCannon.power;
    fruits.emplace_back(fruitPosition, fruitVelocity, FruitType::APPLE);
    leftCannon.power = 0.0;
}

void Ship::fireRightCannon(std::vector<Fruit> &fruits) {
    Vector2D fruitPosition = rightCannonPosition();
    Vector2D fruitVelocity = velocity;
    fruitVelocity.x += cos(angle + PI / 2) * rightCannon.power;
    fruitVelocity.y += sin(angle + PI / 2) * rightCannon.power;
    fruits.emplace_back(fruitPosition, fruitVelocity, FruitType::APPLE);
    rightCannon.power = 0.0;
}

void Ship::handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits) {
    if (fire_left->is_targeted(key, mouse) and isChargingLeft) {
        if (leftCannon.power > MIN_POWER) {
            fireLeftCannon(fruits);
        }
        isChargingLeft = false;
        leftCannon.power = 0.0;
    }
    if (fire_right->is_targeted(key, mouse) and isChargingRight) {
        if (rightCannon.power > MIN_POWER) {
            fireRightCannon(fruits);
        }
        isChargingRight = false;
        rightCannon.power = 0.0;
    }
}

void Ship::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (fire_left->is_targeted(key, mouse) and not isChargingLeft) {
        isChargingLeft = true;
    }
    if (fire_right->is_targeted(key, mouse) and not isChargingRight) {
        isChargingRight = true;
    }
}

void Ship::handle_collision(Ship &other) {
    if (Entity::intersects(other)) {
        Vector2D vec = other.position;
        vec.subtract(position);

        double power = velocity.length() + other.velocity.length();
        acceleration.add_scaled(vec, -power * 0.5);
        other.acceleration.add_scaled(vec, power * 0.5);
    }
}


Cannon::Cannon(const int shipWidth) : texture(&textureHandler.getTexture(TextureID::CANNON)) {}

void Cannon::render(const int x, const int y, const double angle) const {
    texture->render(x, y, angle + PI / 2);
}


