#ifndef FRUITSHARK_SOUND_H
#define FRUITSHARK_SOUND_H
#include <SDL.h>
#include <SDL_mixer.h>
#include "engine/exceptions.h"

class sound_exception : public base_exception {
public:
    explicit sound_exception(std::string msg) : base_exception(std::move(msg)) {};

};

namespace sound {
    enum Id {
        WATER, BITE, CANNON
    };

    void play(Id id);

    void init();

    void free();
};




#endif //FRUITSHARK_SOUND_H
