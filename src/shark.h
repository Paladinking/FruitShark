#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H
#include "engine/texture.h"
#include "vector2D.h"

class Shark {
public:
    Shark(double x, double y);

    void tick(double delta);

    void render();
private:
    Vector2D position;
    Texture texture;
};


#endif //FRUITSHARK_SHARK_H
