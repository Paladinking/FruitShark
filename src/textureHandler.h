#ifndef FRUITSHARK_TEXTUREHANDLER_H
#define FRUITSHARK_TEXTUREHANDLER_H

#include "engine/texture.h"

enum class TextureID {
    SHIP = 0,
    CANNON = 1,
    SHARK1 = 2,
    SHARK2 = 3,
    SHARK3 = 4,
    APPLE1 = 5,
    APPLE2 = 6
};

class TextureHandler {
public:
    TextureHandler() = default;

    void loadTextures();

    [[nodiscard]] const Texture& getTexture(TextureID texture) const;
private:
    Texture shipTexture;
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
