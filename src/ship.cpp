#include "Ship.h"
#include "engine/engine.h"

constexpr double ACCELERATION = 800.0;
constexpr double FRICTION = 0.02;
constexpr double ABSOLUTE_FRICTION_THRESHOLD = 5.0;

Ship::Ship(double x, double y, const char *const *bindings) :
position(x, y),
leftCannon(width, length, false),
rightCannon(width, length, true) {
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
    texture.render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    int cannonX = static_cast<int>(position.x - (width * cos((angle + 90.0) * PI / 180.0)) / 1.5);
    int cannonY = static_cast<int>(position.y - (width * sin((angle + 90.0) * PI / 180.0)) / 1.5);
    leftCannon.render(cannonX, cannonY, angle);

    cannonX = static_cast<int>(position.x + (width * cos((angle + 90.0) * PI / 180.0)) / 1.5);
    cannonY = static_cast<int>(position.y + (width * sin((angle + 90.0) * PI / 180.0)) / 1.5);
    rightCannon.render(cannonX, cannonY, angle);
}

Cannon::Cannon(const int shipWidth, const int shipLength, const bool isRightCannon) : isRightCannon(isRightCannon) {
    int cannonWidth = static_cast<int>(shipWidth / 4);
    int cannonLength = static_cast<int>(shipLength / 4);
    texture.load_from_file("Cannon.png", cannonWidth, cannonLength);
}

void Cannon::render(const int x, const int y, const double angle) const {
    texture.render(x, y, angle);
}