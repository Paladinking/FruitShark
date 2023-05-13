#ifndef FRUITSHARK_ENTITY_H
#define FRUITSHARK_ENTITY_H
#include "vector2D.h"

class Entity {
public:
    Entity(double x, double y);
protected:
    Vector2D position, velocity {}, acceleration {};

    void move(double delta);
};


#endif //FRUITSHARK_ENTITY_H
