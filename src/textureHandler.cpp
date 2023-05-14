#include "textureHandler.h"
#include "engine/exceptions.h"

void TextureHandler::loadTextures() {
    int shipWidth = 75;
    int shipHeight = 37;
    shipTexture.load_from_file("assets/images/Ship.png", shipWidth, shipHeight);
    mastsTexture.load_from_file("assets/images/Masts.png", shipWidth, shipHeight);
    sailsWhiteTexture.load_from_file("assets/images/Sails_white.png", shipWidth, shipHeight);
    sailsBlueTexture.load_from_file("assets/images/Sails_blue.png", shipWidth, shipHeight);
    sailsGreenTexture.load_from_file("assets/images/Sails_green.png", shipWidth, shipHeight);
    sailsRedTexture.load_from_file("assets/images/Sails_red.png", shipWidth, shipHeight);
    cannonTexture.load_from_file("assets/images/Cannon.png", 17, 12);

    int sharkWidth = 100;
    int sharkHeight = 50;
    sharkTexture[0].load_from_file("assets/images/Shark1.png", sharkWidth, sharkHeight);
    sharkTexture[1].load_from_file("assets/images/Shark2.png", sharkWidth, sharkHeight);
    sharkTexture[2].load_from_file("assets/images/Shark3.png", sharkWidth, sharkHeight);

    whiteSharkTexture[0].load_from_file("assets/images/GreatWhite1.png", 140, 70);
    whiteSharkTexture[1].load_from_file("assets/images/GreatWhite2.png", 140, 70);
    whiteSharkTexture[2].load_from_file("assets/images/GreatWhite3.png", 140, 70);

    int fruitSize = 20;
    appleTexture[0].load_from_file("assets/images/Apple1.png", fruitSize, fruitSize);
    appleTexture[1].load_from_file("assets/images/Apple2.png", fruitSize, fruitSize);

    bananaTexture[0].load_from_file("assets/images/Banana1.png", fruitSize, fruitSize);
    bananaTexture[1].load_from_file("assets/images/Banana2.png", fruitSize, fruitSize);

    teethTexture[0].load_from_file("assets/images/Teeth1.png", 75, 75);
    teethTexture[1].load_from_file("assets/images/Teeth2.png", 75, 75);
    teethTexture[2].load_from_file("assets/images/Teeth3.png", 75, 75);
    teethTexture[3].load_from_file("assets/images/Teeth4.png", 75, 75);
    teethTexture[4].load_from_file("assets/images/Teeth5.png", 75, 75);
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
        case TextureID::WHITE_SHARK:
            return whiteSharkTexture;
        default:
            throw image_load_exception("Invalid texture id");

    }
}


