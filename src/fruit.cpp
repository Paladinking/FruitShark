#include "fruit.h"

const Texture* get_fruit_texture(FruitType type) {
    switch (type) {
        case FruitType::APPLE:
            return texture_handler.get_textures(TextureID::APPLE);
        case FruitType::BANANA:
            return texture_handler.get_textures(TextureID::BANANA);
        case FruitType::POMEGRANATE:
            return texture_handler.get_textures(TextureID::POMEGRANATE);
        case FruitType::POMEGRANATE_SEED:
            return texture_handler.get_textures(TextureID::POMEGRANATE_SEED);
        default:
            return texture_handler.get_textures(TextureID::APPLE);
    }
}


Fruit::Fruit(Vector2D position, Vector2D velocity, FruitType type = FruitType::APPLE) :
position(position),
velocity(velocity),
type(type),
texture(get_fruit_texture(type))
 {
    if (type == FruitType::BANANA) acceleration.add_scaled(velocity, -2);
    switch (type) {
        case FruitType::APPLE:
            max_time_in_air = 0.5;
            break;
        case FruitType::BANANA:
            max_time_in_air = 1.5;
            break;
        case FruitType::POMEGRANATE:
            max_time_in_air = 0.4;
            break;
        case FruitType::POMEGRANATE_SEED:
            max_time_in_air = 0.2;
            break;
        default:
            max_time_in_air = 0.0;
    }
     max_velocity = velocity.length();
}

void Fruit::tick(double delta, std::vector<Fruit> &fruits) {
    time_in_air += delta;
    if (time_in_air > max_time_in_air) {
        land(fruits);
        acceleration.scale(0);
    }
    switch (type) {
        case FruitType::BANANA:
            angle += acceleration.length() * delta / 64;
            if (velocity.length() <= max_velocity) velocity.add_scaled(acceleration, delta);
            position.add_scaled(velocity, delta);
            break;
        default:
            angle += velocity.length() * delta / 128;
            position.add_scaled(velocity, delta);
            break;
    }
}

void Fruit::render() const {
    (in_water) ? texture[1].render(static_cast<int>(position.x), static_cast<int>(position.y), angle) :
    texture[0].render(static_cast<int>(position.x), static_cast<int>(position.y), angle);
}

void Fruit::land(std::vector<Fruit> &fruits) {
    in_water = true;
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
         case FruitType::POMEGRANATE_SEED:
             return 5.0;
         default:
             return 10;
     }
}

double Fruit::get_duration() {
    switch (type) {
        case FruitType::APPLE:
            return 3.0;
        case FruitType::BANANA:
            return 6.0;
            break;
        case FruitType::POMEGRANATE:
            return 5.0;
        case FruitType::POMEGRANATE_SEED:
            return 1.5;
        default:
            return 0.0;
    }
}
