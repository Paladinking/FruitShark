#include "pickup.h"

Pickup::Pickup(double x, double y, FruitType type) :
        Entity(x,y, engine::random(0, 5) * 3.1415 / 5.0),
        type(type), time_alive(0.0),
        box_texture(&texture_handler.get_texture(TextureID::BOX)),
        fruit_texture(get_fruit_texture(type)) {}

void Pickup::tick_physics(double delta) {
    angle += delta;
    time_alive += delta;
}

void Pickup::render() const {
    box_texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    fruit_texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

FruitType Pickup::get_type() {
    return type;
}

const Vector2D &Pickup::get_position() const {
    return position;
}

double Pickup::get_radius() const {
    return 15.0;
}
