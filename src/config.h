#ifndef FRUITSHARK_CONFIG_H
#define FRUITSHARK_CONFIG_H
#include "textureHandler.h"
#include <vector>
#include "vector2D.h"

constexpr int UI_SIZE = 80;
constexpr int LOGICAL_WIDTH = 1920, LOGICAL_HEIGHT = 1080;
constexpr int GAME_WIDTH = LOGICAL_WIDTH - 2 * UI_SIZE, GAME_HEIGHT = LOGICAL_HEIGHT;

constexpr unsigned PLAYER_COUNT = 2;

constexpr unsigned INITIAL_SHARK_COUNT = 10;
constexpr double PICKUP_SPAWN_TIME = 10.0;

constexpr const char*  BINDINGS[2][6] = {
        {"W", "A", "S", "D", "V", "B"},
        {"Up", "Left", "Down", "Right", ",", "."}
};

constexpr TextureID COLORS[4] = {TextureID::SAILS_WHITE, TextureID::SAILS_BLUE,
                                 TextureID::SAILS_GREEN, TextureID::SAILS_RED};
constexpr int START_X[4][4] = {
        {UI_SIZE + LOGICAL_WIDTH /2, 0, 0, 0 },
        {UI_SIZE + 150, LOGICAL_WIDTH - UI_SIZE - 150, 0, 0},
        {UI_SIZE + 150, LOGICAL_WIDTH - UI_SIZE - 150, UI_SIZE + GAME_WIDTH / 2, 0},
        {UI_SIZE + 150, LOGICAL_WIDTH - UI_SIZE - 150, UI_SIZE + 150, LOGICAL_WIDTH - UI_SIZE - 150}
};

constexpr int START_Y[4][4] = {
        {LOGICAL_HEIGHT / 2, 0, 0, 0},
        {LOGICAL_HEIGHT / 2, LOGICAL_HEIGHT / 2, 0, 0},
        {3 * LOGICAL_HEIGHT / 4 , LOGICAL_HEIGHT / 4, 3 * LOGICAL_HEIGHT / 4, 0},
        { LOGICAL_HEIGHT / 4, LOGICAL_HEIGHT / 4, 3 * LOGICAL_HEIGHT / 4, 3 * LOGICAL_HEIGHT / 4}
};
extern TextureHandler texture_handler;

const std::vector<std::vector<Vector2D>>& get_shark_trails();


#endif //FRUITSHARK_CONFIG_H
