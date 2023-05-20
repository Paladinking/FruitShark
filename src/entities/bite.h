#ifndef FRUITSHARK_BITE_H
#define FRUITSHARK_BITE_H
#include "engine/texture.h"

class Bite {
public:
    Bite(int x, int y);


    void tick(double delta);

    void render() const;

    bool is_dead() const;
private:
    int x,  y;
    const Texture* textures;

    double passed = 0.0;
};


#endif //FRUITSHARK_BITE_H
