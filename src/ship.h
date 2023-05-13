#ifndef FRUITSHARK_SHIP_H
#define FRUITSHARK_SHIP_H
#include "entity.h"
#include "engine/texture.h"
#include "engine/input.h"

 constexpr double PI = 3.14159265;

class Cannon {
public:
    Cannon(int shipWidth);

    void render(int x, int y, double angle) const;

    //void fire();

private:
    Texture texture;
};

class Ship : Entity {
public:
    Ship(double x, double y, const char *const *bindings);

    void render() const;

    void tick(double delta, const Uint8* keyboard, Uint32 mouse_mask);

private:

    int width = 25;
    int length = 50;
    double angle = 0.0;

    Texture texture;

    std::unique_ptr<HoldInput> forward, back, left, right;

    Vector2D leftCannonPosition() const;
    Vector2D rightCannonPosition() const;

    Cannon leftCannon;
    Cannon rightCannon;
};


#endif //FRUITSHARK_SHIP_H
