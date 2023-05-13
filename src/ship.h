#ifndef FRUITSHARK_SHIP_H
#define FRUITSHARK_SHIP_H
#include "vector2D.h"
#include "engine/texture.h"
#include "engine/input.h"

 constexpr double PI = 3.14159265;

class Cannon {
public:
    Cannon();

    void render(int shipX, int shipY, int shipWidth, int shipLength, double shipAngle, bool isRightCannon) const;

    //void fire();

private:
    Texture texture;
};

class Ship {
public:
    Ship(double x, double y, const char *const *bindings);

    void render() const;

    void tick(double delta, const Uint8* keyboard, Uint32 mouse_mask);

private:
    Vector2D position;
    Vector2D velocity {};
    Vector2D acceleration {};

    int width;
    int length;
    double angle = 0.0;

    Texture texture;

    std::unique_ptr<HoldInput> forward, back, left, right;

    Cannon leftCannon;
    Cannon rightCannon;
};


#endif //FRUITSHARK_SHIP_H
