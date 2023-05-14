#include "textureHandler.h"
#include "engine/exceptions.h"

void TextureHandler::loadTextures() {
    int shipWidth = 75;
    int shipHeight = 37;
    shipTexture.load_from_file("Ship.png", shipWidth, shipHeight);
    mastsTexture.load_from_file("Masts.png", shipWidth, shipHeight);
    sailsWhiteTexture.load_from_file("Sails_white.png", shipWidth, shipHeight);
    sailsBlueTexture.load_from_file("Sails_blue.png", shipWidth, shipHeight);
    sailsGreenTexture.load_from_file("Sails_green.png", shipWidth, shipHeight);
    sailsRedTexture.load_from_file("Sails_red.png", shipWidth, shipHeight);
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
        case TextureID::MASTS:
            return mastsTexture;
        case TextureID::SAILS_WHITE:
            return sailsWhiteTexture;
        case TextureID::SAILS_BLUE:
            return sailsBlueTexture;
        case TextureID::SAILS_GREEN:
            return sailsGreenTexture;
        case TextureID::SAILS_RED:
            return sailsRedTexture;
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


