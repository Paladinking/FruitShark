#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H
#include "engine/texture.h"
#include "entity.h"
#include <vector>

class Shark : Entity {
public:
    Shark(double x, double y);

    void tick(double delta, const std::vector<std::vector<Vector2D>>& trails);

    void render() const;

    void set_trail(const std::vector<Vector2D>* new_trail);
private:
    Texture texture;

    const std::vector<Vector2D>* trail = nullptr;
    unsigned trail_index = 0;
};


#endif //FRUITSHARK_SHARK_H
