#include "fruit.h"


Fruit::Fruit(Vector2D position, Vector2D velocity, FruitType type = FruitType::APPLE) :
position(position),
velocity(velocity),
type(type),
texture1(&textureHandler.getTexture(static_cast<TextureID>(TextureID::APPLE1))),
texture2(&textureHandler.getTexture(static_cast<TextureID>(TextureID::APPLE2))) {
}

void Fruit::tick(double delta) {
    timeAlive += delta;
    if (timeAlive > 2 and not inWater) {
        land();
    } else if(timeAlive > 10) {
        eaten = true;
    }
    angle += velocity.length() * delta / 128;
    switch (type) {
        default:
            position.add_scaled(velocity, delta);
            //position.x += velocity.x * delta;
            //position.y += velocity.y * delta;
            break;
    }
}

void Fruit::render() const {
    switch (type) {
        case FruitType::APPLE:
            if (inWater) {
                texture2->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            } else {
                texture1->render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
            }
            break;
        default:
            SDL_Rect r = {static_cast<int>(position.x - 5), static_cast<int>(position.y - 5), 10, 10};
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x50, 0x50, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
            break;
    }
}

void Fruit::land() {
    inWater = true;
    velocity.scale(0.05);
}

const Vector2D &Fruit::getPosition() const {
    return position;
}
