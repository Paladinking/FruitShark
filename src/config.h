#ifndef FRUITSHARK_CONFIG_H
#define FRUITSHARK_CONFIG_H
#include "textureHandler.h"

constexpr int UI_SIZE = 80;
constexpr int LOGICAL_WIDTH = 1920, LOGICAL_HEIGHT = 1080;
constexpr int GAME_WIDTH = LOGICAL_WIDTH - 2 * UI_SIZE, GAME_HEIGHT = LOGICAL_HEIGHT;

constexpr unsigned PLAYER_COUNT = 2;

constexpr unsigned INITIAL_SHARK_COUNT = 10;

constexpr const char*  BINDINGS[2][6] = {
        {"W", "A", "S", "D", "V", "B"},
        {"I", "J", "K", "L", "T", "Y"}
};

constexpr TextureID COLORS[4] = {TextureID::SAILS_WHITE, TextureID::SAILS_BLUE,
                                 TextureID::SAILS_GREEN, TextureID::SAILS_RED};

extern TextureHandler textureHandler;


#endif //FRUITSHARK_CONFIG_H
