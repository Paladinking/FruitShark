#include "fruit.h"


Fruit::Fruit(Vector2D position, Vector2D velocity, FruitType type = FruitType::APPLE) :
position(position),
velocity(velocity),
type(type),
texture(&textureHandler.getTexture(static_cast<TextureID>(type))) {
}

void Fruit::tick(double delta) {
    timeAlive += delta;
    if (timeAlive > 2) {
        kill();
    }
    angle += velocity.length() * delta / 128;
    switch (type) {
        default:
            position.x += velocity.x * delta;
            position.y += velocity.y * delta;
            break;
    }
}

void Fruit::render() const {
    switch (type) {
        case FruitType::APPLE:
            texture->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            break;
        default:
            SDL_Rect r = {static_cast<int>(position.x - 5), static_cast<int>(position.y - 5), 10, 10};
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x50, 0x50, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
            break;
    }
}

void Fruit::kill() {
    dead = true;
}
