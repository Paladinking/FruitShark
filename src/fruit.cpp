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
        case FruitType::POMEGRANATE:
            maxTimeInAir = 0.4;
            break;
        case FruitType::POMEGRANATE_SEED:
            maxTimeInAir = 0.2;
            break;  
        default:
            maxTimeInAir = 0.0;
    }
    maxVelocity = velocity.length();
}

void Fruit::tick(double delta, std::vector<Fruit> &fruits) {
    timeInAir += delta;
    if (timeInAir > maxTimeInAir) {
        land(fruits);
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

void Fruit::land(std::vector<Fruit> &fruits) {
    inWater = true;
    velocity.scale(0.05);
    if (type == FruitType::POMEGRANATE) {
        for (int i = 0; i < 5; ++i) {
            int a = engine::random(0, 360 * 100);
            double dir = a * 3.14159265 / (180.0 * 100);
            Vector2D vel = {500.0, 0.0};
            vel.rotate(dir);
            fruits.emplace_back(position, vel, FruitType::POMEGRANATE_SEED);
        }
        eaten = true;
    }
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
