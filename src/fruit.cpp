#include "fruit.h"

const Texture* getFruitTexture(FruitType type) {
    switch (type) {
        case FruitType::APPLE:
            return textureHandler.getTextures(TextureID::APPLE);
        case FruitType::BANANA:
            return textureHandler.getTextures(TextureID::BANANA);
        default:
            return textureHandler.getTextures(TextureID::APPLE);
    }
}


Fruit::Fruit(Vector2D position, Vector2D velocity, FruitType type = FruitType::APPLE) :
position(position),
velocity(velocity),
type(type),
texture(getFruitTexture(type))
 {
    if (type == FruitType::BANANA) acceleration.add_scaled(velocity, -2);
    switch (type) {
        case FruitType::APPLE:
            maxTimeInAir = 0.5;
            break;
        case FruitType::BANANA:
            maxTimeInAir = 1.5;
            break;
        default:
            maxTimeInAir = 0.0;
    }
    maxVelocity = velocity.length();
}

void Fruit::tick(double delta) {
    timeInAir += delta;
    if (timeInAir > maxTimeInAir) {
        land();
        acceleration.scale(0);
    }
    switch (type) {
        case FruitType::BANANA:
            angle += acceleration.length() * delta / 64;
            if (velocity.length() <= maxVelocity) velocity.add_scaled(acceleration, delta);
            position.add_scaled(velocity, delta);
            break;
        default:
            angle += velocity.length() * delta / 128;
            position.add_scaled(velocity, delta);
            break;
    }
}

void Fruit::render() const {
    (inWater) ? texture[1].render(static_cast<int>(position.x), static_cast<int>(position.y), angle) :
                texture[0].render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

void Fruit::land() {
    inWater = true;
    velocity.scale(0.05);
}

const Vector2D &Fruit::get_position() const {
    return position;
}
 double Fruit::get_radius() const {
     switch (type) {
         case FruitType::APPLE:
             return 7.5;
         default:
             return 10;
     }
}
