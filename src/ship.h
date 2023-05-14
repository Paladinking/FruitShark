#ifndef FRUITSHARK_SHIP_H
#define FRUITSHARK_SHIP_H

#include <vector>
#include "entity.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "fruit.h"

constexpr int MAX_HP = 100;

class Cannon {
public:
    explicit Cannon(int shipWidth);

    void render(int x, int y, double angle) const;

    double power = 0.0;
private:
    const Texture* texture;
};



class Ship : public Entity {
public:
    Ship(double x, double y, const char *const *bindings);

    void render() const;

    void tick(double delta, const Uint8* keyboard, Uint32 mouse_mask, std::vector<Fruit>& fruits);

    void handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits);

    void handle_down(SDL_Keycode key, Uint8 mouse);

    void handle_collision(Ship &other);

    void get_bitten(int damage);

private:
    const Texture* texture;

    std::unique_ptr<HoldInput> forward, back, left, right;
    std::unique_ptr<PressInput> fire_left, fire_right;

    bool isChargingLeft = false;
    bool isChargingRight = false;

    int hp = MAX_HP;

    [[nodiscard]] Vector2D leftCannonPosition() const;
    [[nodiscard]] Vector2D rightCannonPosition() const;

    void fireLeftCannon(std::vector<Fruit>& fruits);
    void fireRightCannon(std::vector<Fruit>& fruits);

    Cannon leftCannon;
    Cannon rightCannon;
};


#endif //FRUITSHARK_SHIP_H
