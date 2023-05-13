#ifndef FRUITSHARK_TEXTUREHANDLER_H
#define FRUITSHARK_TEXTUREHANDLER_H

#include "engine/texture.h"

enum class TextureID {
    SHIP = 0,
    CANNON = 1,
    SHARK1 = 2,
    SHARK2 = 3,
    SHARK3 = 4,
    APPLE = 5
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
    Texture appleTexture;
    Texture bananaTexture;
    Texture starfruitTexture;
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
