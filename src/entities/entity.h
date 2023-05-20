#ifndef FRUITSHARK_ENTITY_H
#define FRUITSHARK_ENTITY_H
#include "vector2D.h"
#include "net/serialize.h"
#include <SDL.h>

constexpr double PI = 3.14159265;

class Entity : public Serializable {
public:
    Entity(double x, double y, double angle);

    [[nodiscard]] size_t size() const override;

    void write(unsigned char* buf) const override;

    void read(const unsigned char* buf) override;
protected:
    Vector2D position;
    double angle = 0.0;
};

class BoxEntity : public Entity {
public:
    BoxEntity(double x, double y, int width, int length);
    BoxEntity(double x, double y, int width, int length, double angle);

    [[nodiscard]] bool intersects(const BoxEntity& other) const;

    [[nodiscard]] bool intersects(Vector2D pos, double radius) const;

    void handle_Collision(BoxEntity& other);

    [[nodiscard]] const Vector2D& get_position() const;
protected:
    Vector2D velocity {}, acceleration {};

    int width;
    int length;

    void move(double delta);

protected:
    void update_bounds();

    void adjust_bounds(float dx, float dy);

    SDL_FPoint corners[4] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    SDL_FRect bounds = {0.0, 0.0, 0.0, 0.0};
};


#endif //FRUITSHARK_ENTITY_H
