#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H

#include "engine/texture.h"
#include "engine/engine.h"
#include "entity.h"
#include <vector>
#include "config.h"
#include "fruit.h"
#include "ship.h"

class Shark : public Entity {
public:
    Shark(double x, double y);

    void tick(double delta, const std::vector<std::vector<Vector2D>>& trails,
              const std::vector<Fruit>& fruitsInWater,
              const std::vector<Ship>& ships
              );

    void render() const;

    [[nodiscard]] Vector2D get_mouth() const;

    void set_trail(const std::vector<Vector2D>* new_trail);

    bool bite(Ship& ship);
private:
    const Texture* texture;
    double animationStage;

    const std::vector<Vector2D>* trail = nullptr;
    unsigned trail_index = 0;

    double bite_delay = 0.0;
};


#endif //FRUITSHARK_SHARK_H
