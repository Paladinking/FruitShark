#ifndef FRUITSHARK_SHIP_H
#define FRUITSHARK_SHIP_H

#include <vector>
#include "entity.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "fruit.h"

constexpr int MAX_HP = 50;
constexpr double ACCELERATION = 800.0;
constexpr double MIN_POWER = 750.0;
constexpr double MAX_POWER = 1250.0;
constexpr double POWER_PER_SECOND = 1000.0;
constexpr double COOLDOWN_TIME = 0.5;

class Cannon {
public:
    explicit Cannon();

    void render(int x, int y, double angle) const;

    double power = 0.0;
    double cooldown = 0.0;
private:
    const Texture* texture;
};



class Ship : public BoxEntity {
public:
    Ship(double x, double y, TextureID sail_color, int id, double angle);

    void render() const;

    void tick(double delta, const bool* inputs, std::vector<Fruit>& fruits);

    void handle_up(bool left, bool right, std::vector<Fruit>& fruits);

    void handle_down(bool left, bool right);

    void get_bitten(int damage);

    [[nodiscard]]bool has_fruit_smell() const;

    void add_fruit_smell(double duration);

    void add_fruits(FruitType type, int count);

    bool is_dead() const;

    int id;

    enum class Direction : Uint8 {
        FORWARDS = 0, LEFT = 1, RIGHT = 3
    };

private:
    const Texture* ship_texture;
    const Texture* masts_texture;
    const Texture* sails_texture;

    bool is_charging_left = false;
    bool is_charging_right = false;

    int hp = MAX_HP;

    double smell_duration = 0.0;

    int fruit_count = -1;
    FruitType fruit_type = FruitType::APPLE;

    [[nodiscard]] Vector2D left_cannon_position() const;
    [[nodiscard]] Vector2D right_cannon_position() const;

    void fire_left_cannon(std::vector<Fruit>& fruits);
    void fire_right_cannon(std::vector<Fruit>& fruits);

    Cannon left_cannon;
    Cannon right_cannon;
};


#endif //FRUITSHARK_SHIP_H
