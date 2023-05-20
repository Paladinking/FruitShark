#ifndef FRUITSHARK_SHARK_H
#define FRUITSHARK_SHARK_H

#include "engine/texture.h"
#include "engine/engine.h"
#include "entities/entity.h"
#include <vector>
#include "config.h"
#include "entities/fruit.h"
#include "ship.h"


class Shark : public BoxEntity {
public:
    const enum class Type : Uint8{
        GREAT_WHITE,
        TIGER
    } type;
    static Shark create_shark(Type type, double x, double y);

    void tick_animation(double delta);

    void tick_physics(double delta, const std::vector<std::vector<Vector2D>>& trails,
              const std::vector<Fruit>& fruitsInWater,
              const std::vector<Ship>& ships
              );

    void render() const;

    [[nodiscard]] Vector2D get_mouth() const;

    void set_trail(const std::vector<Vector2D>* new_trail);

    bool bite(Ship& ship);
private:
    Shark(double x, double y, int w, int len, int dmg, double range_factor, double acc_factor, Type type, TextureID id);

    int dmg;

    double range_factor;
    double acc_factor;

    const Texture* texture;
    double animation_stage;

    const std::vector<Vector2D>* trail = nullptr;
    unsigned trail_index = 0;

    double bite_delay = 0.0;
};


#endif //FRUITSHARK_SHARK_H
