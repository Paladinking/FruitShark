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

    pomegranateTexture[0].load_from_file("assets/images/Pomegranate1.png", fruitSize, fruitSize);
    pomegranateTexture[1].load_from_file("assets/images/Pomegranate1.png", fruitSize, fruitSize);

    pomegranateSeedTexture[0].load_from_file("assets/images/Seed1.png", fruitSize / 2, fruitSize / 2);
    pomegranateSeedTexture[1].load_from_file("assets/images/Seed2.png", fruitSize / 2, fruitSize / 2);

    int boxSize = fruitSize + 10;
    boxTexture.load_from_file("assets/images/PickupBox.png", boxSize, boxSize);

    int teethSize = 75;
    teethTexture[0].load_from_file("assets/images/Teeth1.png", teethSize, teethSize);
    teethTexture[1].load_from_file("assets/images/Teeth2.png", teethSize, teethSize);
    teethTexture[2].load_from_file("assets/images/Teeth3.png", teethSize, teethSize);
    teethTexture[3].load_from_file("assets/images/Teeth4.png", teethSize, teethSize);
    teethTexture[4].load_from_file("assets/images/Teeth5.png", teethSize, teethSize);
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
        case TextureID::BOX:
            return boxTexture;
        case TextureID::APPLE:
            return appleTexture[0];
        case TextureID::BANANA:
            return bananaTexture[0];
        case TextureID::POMEGRANATE:
            return pomegranateTexture[0];
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
        case TextureID::POMEGRANATE:
            return pomegranateTexture;
        case TextureID::POMEGRANATE_SEED:
            return pomegranateSeedTexture;
        case TextureID::WHITE_SHARK:
            return whiteSharkTexture;
        default:
            throw image_load_exception("Invalid texture id");

    }
}


