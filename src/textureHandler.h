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
    SHARK,
    WHITE_SHARK,
    APPLE,
    BANANA,
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
    Texture sharkTexture[3];
    Texture appleTexture[2];
    Texture bananaTexture[2];
    Texture whiteSharkTexture[3];
    Texture starfruitTexture[2];
    Texture teethTexture[5];
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
