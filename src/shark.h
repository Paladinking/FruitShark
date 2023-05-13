#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H
#include "engine/texture.h"
#include "entity.h"
#include <vector>

class Shark : Entity {
public:
    Shark(double x, double y);

    void tick(double delta);

    void render() const;
private:
    Texture texture;

    std::vector<Vector2D>* trail = nullptr;
    unsigned trail_index = 0;
};


#endif //FRUITSHARK_SHARK_H
