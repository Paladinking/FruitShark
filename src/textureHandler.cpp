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
    sharkTexture[0].load_from_file("Shark1.png", sharkWidth, sharkHeight);
    sharkTexture[1].load_from_file("Shark2.png", sharkWidth, sharkHeight);
    sharkTexture[2].load_from_file("Shark3.png", sharkWidth, sharkHeight);

    int fruitSize = 20;
    appleTexture[0].load_from_file("Apple1.png", fruitSize, fruitSize);
    appleTexture[1].load_from_file("Apple2.png", fruitSize, fruitSize);

    bananaTexture[0].load_from_file("Banana1.png", fruitSize, fruitSize);
    bananaTexture[1].load_from_file("Banana2.png", fruitSize, fruitSize);

    teethTexture[0].load_from_file("Teeth1.png", 75, 75);
    teethTexture[1].load_from_file("Teeth2.png", 75, 75);
    teethTexture[2].load_from_file("Teeth3.png", 75, 75);
    teethTexture[3].load_from_file("Teeth4.png", 75, 75);
    teethTexture[4].load_from_file("Teeth5.png", 75, 75);
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
        default:
            throw image_load_exception("Invalid texture id");
    }
}

const Texture *TextureHandler::getTextures(TextureID texture) const {
    switch (texture) {
        case TextureID::SHARK:
            return sharkTexture;
        case TextureID::TEETH:
            return teethTexture;
        case TextureID::BANANA:
            return bananaTexture;
        case TextureID::APPLE:
            return appleTexture;
        default:
            throw image_load_exception("Invalid texture id");

    }
}


