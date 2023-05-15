#include "textureHandler.h"
#include "engine/exceptions.h"

void TextureHandler::load_textures() {
    int ship_width = 75;
    int ship_height = 37;
    ship_texture.load_from_file("assets/images/Ship.png", ship_width, ship_height);
    masts_texture.load_from_file("assets/images/Masts.png", ship_width, ship_height);
    sails_white_texture.load_from_file("assets/images/Sails_white.png", ship_width, ship_height);
    sails_blue_texture.load_from_file("assets/images/Sails_blue.png", ship_width, ship_height);
    sails_green_texture.load_from_file("assets/images/Sails_green.png", ship_width, ship_height);
    sails_red_texture.load_from_file("assets/images/Sails_red.png", ship_width, ship_height);
    cannon_texture.load_from_file("assets/images/Cannon.png", 17, 12);

    int shark_width = 100;
    int shark_height = 50;
    shark_texture[0].load_from_file("assets/images/Shark1.png", shark_width, shark_height);
    shark_texture[1].load_from_file("assets/images/Shark2.png", shark_width, shark_height);
    shark_texture[2].load_from_file("assets/images/Shark3.png", shark_width, shark_height);

    white_shark_texture[0].load_from_file("assets/images/GreatWhite1.png", 140, 70);
    white_shark_texture[1].load_from_file("assets/images/GreatWhite2.png", 140, 70);
    white_shark_texture[2].load_from_file("assets/images/GreatWhite3.png", 140, 70);

    int fruit_size = 20;
    apple_texture[0].load_from_file("assets/images/Apple1.png", fruit_size, fruit_size);
    apple_texture[1].load_from_file("assets/images/Apple2.png", fruit_size, fruit_size);

    banana_texture[0].load_from_file("assets/images/Banana1.png", fruit_size, fruit_size);
    banana_texture[1].load_from_file("assets/images/Banana2.png", fruit_size, fruit_size);

    pomegranate_texture[0].load_from_file("assets/images/Pomegranate1.png", fruit_size, fruit_size);
    pomegranate_texture[1].load_from_file("assets/images/Pomegranate1.png", fruit_size, fruit_size);

    pomegranate_seed_texture[0].load_from_file("assets/images/Seed1.png", fruit_size / 2, fruit_size / 2);
    pomegranate_seed_texture[1].load_from_file("assets/images/Seed2.png", fruit_size / 2, fruit_size / 2);

    int box_size = fruit_size + 10;
    box_texture.load_from_file("assets/images/PickupBox.png", box_size, box_size);

    int teeth_size = 75;
    teeth_texture[0].load_from_file("assets/images/Teeth1.png", teeth_size, teeth_size);
    teeth_texture[1].load_from_file("assets/images/Teeth2.png", teeth_size, teeth_size);
    teeth_texture[2].load_from_file("assets/images/Teeth3.png", teeth_size, teeth_size);
    teeth_texture[3].load_from_file("assets/images/Teeth4.png", teeth_size, teeth_size);
    teeth_texture[4].load_from_file("assets/images/Teeth5.png", teeth_size, teeth_size);
}

const Texture& TextureHandler::get_texture(TextureID texture) const {
    switch (texture) {
        case TextureID::SHIP:
            return ship_texture;
        case TextureID::MASTS:
            return masts_texture;
        case TextureID::SAILS_WHITE:
            return sails_white_texture;
        case TextureID::SAILS_BLUE:
            return sails_blue_texture;
        case TextureID::SAILS_GREEN:
            return sails_green_texture;
        case TextureID::SAILS_RED:
            return sails_red_texture;
        case TextureID::CANNON:
            return cannon_texture;
        case TextureID::BOX:
            return box_texture;
        case TextureID::APPLE:
            return apple_texture[0];
        case TextureID::BANANA:
            return banana_texture[0];
        case TextureID::POMEGRANATE:
            return pomegranate_texture[0];
        default:
            throw image_load_exception("Invalid texture id");
    }
}

const Texture *TextureHandler::get_textures(TextureID texture) const {
    switch (texture) {
        case TextureID::SHARK:
            return shark_texture;
        case TextureID::TEETH:
            return teeth_texture;
        case TextureID::BANANA:
            return banana_texture;
        case TextureID::APPLE:
            return apple_texture;
        case TextureID::POMEGRANATE:
            return pomegranate_texture;
        case TextureID::POMEGRANATE_SEED:
            return pomegranate_seed_texture;
        case TextureID::WHITE_SHARK:
            return white_shark_texture;
        default:
            throw image_load_exception("Invalid texture id");

    }
}


