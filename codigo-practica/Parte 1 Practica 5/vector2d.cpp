#include "vector2d.h"

vector2D::vector2D(double x, double y) : x(x), y(y) {}

vector2D vector2D::operator+(const vector2D& v) const {
    return vector2D(x + v.x, y + v.y);
}

vector2D vector2D::operator*(double escalar) const {
    return vector2D(x * escalar, y * escalar);
}

double vector2D::modulo() const {
    return std::sqrt(x * x + y * y);
}
