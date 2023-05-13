#include "textureHandler.h"
#include "engine/exceptions.h"

void TextureHandler::loadTextures() {
    shipTexture.load_from_file("Ship.png", 50, 25);
    cannonTexture.load_from_file("Cannon.png", 17, 12);
    appleTexture.load_from_file("APPLE.png", 25, 25);
}

const Texture& TextureHandler::getTexture(TextureID texture) const {
    switch (texture) {
        case TextureID::SHIP:
            return shipTexture;
        case TextureID::CANNON:
            return cannonTexture;
        case TextureID::SHARK:
            return sharkTexture;
        case TextureID::APPLE:
            return appleTexture;
        default:
            throw image_load_exception("Invalid texture id");
    }
}


