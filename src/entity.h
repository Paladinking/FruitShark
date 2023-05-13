#ifndef FRUITSHARK_ENTITY_H
#define FRUITSHARK_ENTITY_H
#include "vector2D.h"

class Entity {
public:
    Entity(double x, double y, int width, int length);
protected:
    Vector2D position, velocity {}, acceleration {};

    double angle = 0.0;

    int width;
    int length;

    void move(double delta);
};


#endif //FRUITSHARK_ENTITY_H
