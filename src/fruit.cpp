#include "fruit.h"


Fruit::Fruit(Vector2D position, Vector2D velocity, FruitType type) :
position(position),
velocity(velocity),
type(type) {

}

void Fruit::render(int x, int y) {
    if (type == Apple) {
        SDL_Rect r = {static_cast<int>(position.x - 5), static_cast<int>(position.y - 5), 10, 10};
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x50, 0x50, 0xFF);
        SDL_RenderFillRect(gRenderer, &r);
    }
}
