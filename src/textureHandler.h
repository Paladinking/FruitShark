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

    void loadTextures();

    [[nodiscard]] const Texture& getTexture(TextureID texture) const;

    [[nodiscard]] const Texture* getTextures(TextureID texture) const;
private:
    Texture shipTexture;
    Texture mastsTexture;
    Texture sailsWhiteTexture;
    Texture sailsBlueTexture;
    Texture sailsGreenTexture;
    Texture sailsRedTexture;
    Texture cannonTexture;
    Texture boxTexture;
    Texture sharkTexture[3];
    Texture whiteSharkTexture[3];
    Texture appleTexture[2];
    Texture bananaTexture[2];
    Texture pomegranateTexture[2];
    Texture pomegranateSeedTexture[2];
    Texture starfruitTexture[2];
    Texture teethTexture[5];
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
