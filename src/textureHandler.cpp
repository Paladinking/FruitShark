#include "textureHandler.h"
#include "engine/exceptions.h"

void TextureHandler::loadTextures() {
    shipTexture.load_from_file("Ship.png", 50, 25);
    cannonTexture.load_from_file("Cannon.png", 17, 12);

    int sharkWidth = 100;
    int sharkHeight = 50;
    sharkTexture1.load_from_file("Shark1.png", sharkWidth, sharkHeight);
    sharkTexture2.load_from_file("Shark2.png", sharkWidth, sharkHeight);
    sharkTexture3.load_from_file("Shark3.png", sharkWidth, sharkHeight);

    int fruitSize = 15;
    appleTexture1.load_from_file("Apple.png", fruitSize, fruitSize);
    appleTexture2.load_from_file("Apple2.png", fruitSize, fruitSize);
}

const Texture& TextureHandler::getTexture(TextureID texture) const {
    switch (texture) {
        case TextureID::SHIP:
            return shipTexture;
        case TextureID::CANNON:
            return cannonTexture;
        case TextureID::SHARK1:
            return sharkTexture1;
        case TextureID::SHARK2:
            return sharkTexture2;
        case TextureID::SHARK3:
            return sharkTexture3;
        case TextureID::APPLE1:
            return appleTexture1;
        case TextureID::APPLE2:
            return appleTexture2;
        default:
            throw image_load_exception("Invalid texture id");
    }
}


