#ifndef FRUITSHARK_SHIP_H
#define FRUITSHARK_SHIP_H

#include <vector>
#include "entity.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "fruit.h"



class Cannon {
public:
    explicit Cannon(int shipWidth);

    void render(int x, int y, double angle) const;

    double power = 0.0;
private:
    Texture texture;
};



class Ship : public Entity {
public:
    Ship(double x, double y, const char *const *bindings);

    void render() const;

    void tick(double delta, const Uint8* keyboard, Uint32 mouse_mask);

    void handle_up(SDL_Keycode key, Uint8 mouse, std::vector<Fruit>& fruits);

    void handle_down(SDL_Keycode key, Uint8 mouse);

    void handle_collision(Ship &other);

private:
    Texture texture;

    std::unique_ptr<HoldInput> forward, back, left, right;
    std::unique_ptr<PressInput> fire_left, fire_right;

    bool has_intersect = false;

    bool isChargingLeft = false;
    bool isChargingRight = false;

    Vector2D leftCannonPosition() const;
    Vector2D rightCannonPosition() const;

    Cannon leftCannon;
    Cannon rightCannon;
};


#endif //FRUITSHARK_SHIP_H
