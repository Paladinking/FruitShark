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
    SHARK1,
    SHARK2,
    SHARK3,
    APPLE1,
    APPLE2,
    BANANA1,
    BANANA2
};

class TextureHandler {
public:
    TextureHandler() = default;

    void loadTextures();

    [[nodiscard]] const Texture& getTexture(TextureID texture) const;
private:
    Texture shipTexture;
    Texture mastsTexture;
    Texture sailsWhiteTexture;
    Texture sailsBlueTexture;
    Texture sailsGreenTexture;
    Texture sailsRedTexture;
    Texture cannonTexture;
    Texture sharkTexture1;
    Texture sharkTexture2;
    Texture sharkTexture3;
    Texture appleTexture1;
    Texture appleTexture2;
    Texture bananaTexture1;
    Texture bananaTexture2;
    Texture starfruitTexture1;
    Texture starfruitTexture2;
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
