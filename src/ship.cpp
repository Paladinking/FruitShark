#include "Ship.h"

constexpr double ACCELERATION = 800.0;


Ship::Ship(double x, double y, const char *const *bindings) :
Entity(x, y),
leftCannon(width),
rightCannon(width) {
    texture.load_from_file("Ship.png", length, width);

    forward = get_hold_input(bindings[0]);
    left = get_hold_input(bindings[1]);
    back = get_hold_input(bindings[2]);
    right = get_hold_input(bindings[3]);

    fire_left = get_press_input(bindings[4]);
    fire_right = get_press_input(bindings[5]);
}

void Ship::tick(double delta, const Uint8 *keyboard, Uint32 mouse_mask) {
    Vector2D vel_delta = { cos(angle * PI / 180.0), sin(angle * PI / 180.0)};
    if (forward->is_pressed(keyboard, mouse_mask)) {
        acceleration.add_scaled(vel_delta,  ACCELERATION);
    }

    if (left->is_pressed(keyboard, mouse_mask)) {
        angle -= 120.0 * delta;
        velocity.rotate(-120.0 * delta * PI / 180.0);
    }
    if (right->is_pressed(keyboard, mouse_mask)) {
        angle += 120.0 * delta;
        velocity.rotate(120.0 * delta * PI / 180.0);
    }
    Entity::move(delta);
}

void Ship::render() const {
    Vector2D cannonPosition = leftCannonPosition();
    leftCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);
    cannonPosition = rightCannonPosition();
    rightCannon.render(static_cast<int>(cannonPosition.x), static_cast<int>(cannonPosition.y), angle);

    texture.render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

Vector2D Ship::leftCannonPosition() const {
    return {position.x - (width * cos((angle + 90.0) * PI / 180.0)) / 1.5,
                    position.y - (width * sin((angle + 90.0) * PI / 180.0)) / 1.5};
}

Vector2D Ship::rightCannonPosition() const {
    return {position.x + (width * cos((angle + 90.0) * PI / 180.0)) / 1.5,
            position.y + (width * sin((angle + 90.0) * PI / 180.0)) / 1.5};
}

void Ship::handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits) {
    if (fire_left->is_targeted(key, mouse) and isChargingLeft) {
        Vector2D fruitPosition = leftCannonPosition();
        Vector2D fruitVelocity = velocity;
        fruitVelocity.x += cos(angle - 90.0) * leftCannon.power;
        fruitVelocity.y += sin(angle - 90.0) * leftCannon.power;

        fruits.emplace_back(fruitPosition, fruitVelocity, Apple);
    }
}

void Ship::handle_down(SDL_Keycode key, Uint8 mouse) {
    if (fire_left->is_targeted(key, mouse)) {
        isChargingLeft = true;
    }
    if (fire_right->is_targeted(key, mouse)) {
        isChargingRight = true;
    }
}



Cannon::Cannon(const int shipWidth) {
    int width = static_cast<int>(shipWidth / 1.5);
    int length = static_cast<int>(shipWidth / 2);
    texture.load_from_file("Cannon.png", width, length);
}

void Cannon::render(const int x, const int y, const double angle) const {
    texture.render(x, y, angle + 90.0);
}


