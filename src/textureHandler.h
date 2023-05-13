#ifndef FRUITSHARK_TEXTUREHANDLER_H
#define FRUITSHARK_TEXTUREHANDLER_H

#include "engine/texture.h"

enum class TextureID {
    SHIP = 0,
    CANNON = 1,
    SHARK = 2,
    APPLE = 3
};

class TextureHandler {
public:
    TextureHandler() = default;

    void loadTextures();

    [[nodiscard]] const Texture& getTexture(TextureID texture) const;
private:
    Texture shipTexture;
    Texture cannonTexture;
    Texture sharkTexture;
    Texture appleTexture;
    Texture bananaTexture;
    Texture starfruitTexture;
};


#endif //FRUITSHARK_TEXTUREHANDLER_H
