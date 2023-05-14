#include "pickup.h"

Pickup::Pickup(int x, int y, FruitType type) :
position(x, y),
type(type),
boxTexture(&textureHandler.getTexture(TextureID::BOX)),
fruitTexture(getFruitTexture(type)) {
    angle = engine::random(0, 5) * 3.1415 / 5.0;
    timeAlive = 0.0;
}

void Pickup::tick(double delta) {
    angle += delta;
    timeAlive += delta;
}

void Pickup::render() const {
    boxTexture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
    fruitTexture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

FruitType Pickup::getType() {
    return type;
}

const Vector2D &Pickup::get_position() const {
    return position;
}

double Pickup::get_radius() const {
    return 15.0;
}
