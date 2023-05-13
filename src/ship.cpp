#include "Ship.h"
#include "engine/engine.h"

constexpr double ACCELERATION = 800.0;
constexpr double FRICTION = 0.02;
constexpr double ABSOLUTE_FRICTION_THRESHOLD = 5.0;

Ship::Ship(double x, double y, const char *const *bindings) :
position(x, y),
leftCannon(width),
rightCannon(width) {
    texture.load_from_file("Ship.png", length, width);

    forward = get_hold_input(bindings[0]);
    left = get_hold_input(bindings[1]);
    back = get_hold_input(bindings[2]);
    right = get_hold_input(bindings[3]);
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

    acceleration.x -= velocity.x * std::abs(velocity.x) * FRICTION;
    acceleration.y -= velocity.y * std::abs(velocity.y) * FRICTION;

    velocity.x += delta * acceleration.x;
    velocity.y += delta * acceleration.y;
    Vector2D to_move = {velocity.x * delta, velocity.y * delta};
    position.add(to_move);
    acceleration.x = 0.0;
    acceleration.y = 0.0;
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



Cannon::Cannon(const int shipWidth) {
    int width = static_cast<int>(shipWidth / 1.5);
    int length = static_cast<int>(shipWidth / 2);
    texture.load_from_file("Cannon.png", width, length);
}

void Cannon::render(const int x, const int y, const double angle) const {
    texture.render(x, y, angle + 90.0);
}