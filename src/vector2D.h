#ifndef FRUITSHARK_VECTOR2D_H
#define FRUITSHARK_VECTOR2D_H
#include <cmath>
/**
 * Class containing a 2-dimensional vector using double.
 */
class Vector2D {
public:
    /**
     * Creates a new vector.
     */
    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double x, double y) : x(x), y(y) {}

    /**
     * Gets the length of this vector.
     */
    [[nodiscard]] double length() const
    {
        return std::sqrt(x * x + y * y);
    }


    /**
     * Gets the squared length of this vector.
     */
    [[nodiscard]] double length_squared() const
    {
        return x * x + y * y;
    }

    /**
     * Gets the distance from this vector to other.
     */
    [[nodiscard]] double distance(const Vector2D &other) const
    {
        return std::sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }

    /**
     * Gets the squared distance from this vector to other.
     */
    [[nodiscard]] double distance_squared(const Vector2D &other) const
    {
        return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
    }

    /**
     * Adds this vector to other.
     */
    void add(Vector2D &other)
    {
        x += other.x;
        y += other.y;
    }

    /**
     * Adds this vector to other using a scaling factor.
     */
    void add_scaled(Vector2D &other, double factor)
    {
        x += other.x * factor;
        y += other.y * factor;
    }

    /**
     * Scales this vector.
     */
    void scale(double factor)
    {
        x *= factor;
        y *= factor;
    }

    /**
     * Normalizes this vector.
     */
    void normalize()
    {
        double len = length();
        if (len == 0.0) {
            return;
        }
        x /= len;
        y /= len;
    }

    /**
     * Returns the dot-product of this vector and other.
     */
    double dot(Vector2D &other) const
    {
        return x * other.x + y * other.y;
    }

    /**
     * Reduces the length of this vector by amount, stopping at 0.
     */
    void decrease(double amount) {
        double len = length();
        double diff = len - amount;
        if (diff <= 0) {
            x = 0;
            y = 0;
            return;
        }
        x = (x / len) * diff;
        y = (y / len) * diff;
    }

    /**
     * Subtracts other vector from this.
     */
    void subtract(Vector2D &other) {
        x -= other.x;
        y -= other.y;
    }

    void rotate(const double angle) {
        double len = length();
        if (len == 0.0) return;
        double a = std::atan2(y, x);
        x = len * cos(a + angle);
        y = len * sin(a + angle);
    }

    [[nodiscard]] double get_angle() const {
        return std::atan2(y, x);
    }

    double x, y;
};
#endif //FRUITSHARK_VECTOR2D_H
