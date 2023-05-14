#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H

#include "engine/texture.h"
#include "engine/engine.h"
#include "entity.h"
#include <vector>
#include "textureHandler.h"
#include "config.h"
#include "fruit.h"
#include "ship.h"

class Shark : public Entity {
public:
    Shark(double x, double y);

    void tick(double delta, const std::vector<std::vector<Vector2D>>& trails, const std::vector<Fruit>& fruitsInWater);

    void render() const;

    void set_trail(const std::vector<Vector2D>* new_trail);

    void bite(Ship& ship);
private:
    const Texture* texture1;
    const Texture* texture2;
    const Texture* texture3;
    int animationStage;

    const std::vector<Vector2D>* trail = nullptr;
    unsigned trail_index = 0;

    double bite_delay = 0.0;
};


#endif //FRUITSHARK_SHARK_H
