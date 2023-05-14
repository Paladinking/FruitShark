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



class Ship : public Entity {
public:
    Ship(double x, double y, const char *const *bindings, TextureID sail_color, int id, double angle);

    void render() const;

    void tick(double delta, const Uint8* keyboard, Uint32 mouse_mask, std::vector<Fruit>& fruits);

    void handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits);

    void handle_down(SDL_Keycode key, Uint8 mouse);

    void get_bitten(int damage);

    [[nodiscard]]bool has_fruit_smell() const;

    void add_fruit_smell(double duration);

    void add_fruits(FruitType type, int count);

    bool is_dead() const;

    int id;

private:
    const Texture* shipTexture;
    const Texture* mastsTexture;
    const Texture* sailsTexture;

    std::unique_ptr<HoldInput> forward, back, left, right;
    std::unique_ptr<PressInput> fire_left, fire_right;

    bool isChargingLeft = false;
    bool isChargingRight = false;

    int hp = MAX_HP;

    double smell_duration = 0.0;

    int fruit_count = -1;
    FruitType fruit_type = FruitType::APPLE;

    [[nodiscard]] Vector2D leftCannonPosition() const;
    [[nodiscard]] Vector2D rightCannonPosition() const;

    void fireLeftCannon(std::vector<Fruit>& fruits);
    void fireRightCannon(std::vector<Fruit>& fruits);

    Cannon leftCannon;
    Cannon rightCannon;
};


#endif //FRUITSHARK_SHIP_H
