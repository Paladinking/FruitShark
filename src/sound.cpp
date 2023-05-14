#include "sound.h"
#include "engine/engine.h"
constexpr int WATER_COUNT = 4;
Mix_Chunk* water[WATER_COUNT];

constexpr int BITE_COUNT = 1;
Mix_Chunk* bite[BITE_COUNT];

constexpr int CANNON_COUNT = 2;
Mix_Chunk* cannon[CANNON_COUNT];

void sound::init() {
    water[0] = Mix_LoadWAV("assets/sound/water1.wav");
    if (water[0] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }
    water[1] = Mix_LoadWAV("assets/sound/water2.wav");
    if (water[1] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }
    water[2] = Mix_LoadWAV("assets/sound/water3.wav");
    if (water[2] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }
    water[3] = Mix_LoadWAV("assets/sound/water4.wav");
    if (water[3] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }

    bite[0] = Mix_LoadWAV("assets/sound/bite.wav");
    if (bite[0] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }

    cannon[0] = Mix_LoadWAV("assets/sound/cannon1.wav");
    if (cannon[0] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }
    cannon[1] = Mix_LoadWAV("assets/sound/cannon2.wav");
    if (cannon[1] == nullptr) {
        throw sound_exception(std::string(Mix_GetError()));
    }
}

void sound::free() {
    for (auto & i : water) {
        Mix_FreeChunk(i);
    }
    for (auto & i : bite) {
        Mix_FreeChunk(i);
    }
}

void sound::play(Id id) {
    int index;
    switch (id) {
        case WATER:
            index = engine::random(0, WATER_COUNT);
            Mix_PlayChannel(-1 , water[index], 0);
            break;
        case BITE:
            index = engine::random(0, BITE_COUNT);
            Mix_PlayChannel(-1, bite[index], 0);
            break;
        case CANNON:
            index = engine::random(0, CANNON_COUNT);
            Mix_PlayChannel(-1, cannon[index], 0);
            break;
        default:
            throw sound_exception("Invalid sound id");
    }
}
