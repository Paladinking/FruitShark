#ifndef FRUITSHARK_TEXTUREHANDLER_H
#define FRUITSHARK_TEXTUREHANDLER_H

#include "engine/texture.h"

enum class TextureID {
    SHIP,
    MASTS,
    SAILS_WHITE,
    SAILS_BLUE,
    SAILS_GREEN,
    SAILS_RED,
    CANNON,
    BOX,
    SHARK,
    WHITE_SHARK,
    APPLE,
    BANANA,
    POMEGRANATE,
    POMEGRANATE_SEED,
    TEETH
};

class TextureHandler {
public:
    TextureHandler() = default;

    void load_textures();

    [[nodiscard]] const Texture& get_texture(TextureID texture) const;

    [[nodiscard]] const Texture* get_textures(TextureID texture) const;
private:
    Texture ship_texture;
    Texture masts_texture;
    Texture sails_white_texture;
    Texture sails_blue_texture;
    Texture sails_green_texture;
    Texture sails_red_texture;
    Texture cannon_texture;
    Texture box_texture;
    Texture shark_texture[3];
    Texture white_shark_texture[3];
    Texture apple_texture[2];
    Texture banana_texture[2];
    Texture pomegranate_texture[2];
    Texture pomegranate_seed_texture[2];
    Texture teeth_texture[5];
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
