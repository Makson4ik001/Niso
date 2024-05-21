#pragma once
#include <ostream>
#include <cmath>
template <typename T>
class Vector2T {
public:
    T x, y;

    constexpr Vector2T() noexcept { x = y = T{}; }

    constexpr Vector2T(T x, T y) noexcept : x(x), y(y) {}

    constexpr Vector2T(Vector2T<T> const& other) noexcept : x(other.x), y(other.y) {}

    constexpr Vector2T(Vector2T<T>&& other) noexcept : x(other.x), y(other.y) {}

    constexpr T dotProduct(Vector2T<T> const& other) const noexcept {
        return x * other.x + y * other.y;
    }

    constexpr T crossProduct(Vector2T<T> const& other) const noexcept {
        return x * other.y - y * other.x;
    }

    constexpr T length() const noexcept {
        return sqrt(dotProduct(*this));
    }

    constexpr Vector2T<T> operator+(Vector2T<T> const& other) const noexcept {
        return Vector2T(x + other.x, y + other.y);
    }

    constexpr Vector2T<T> operator-(Vector2T<T> const& other) const noexcept {
        return Vector2T(x - other.x, y - other.y);
    }

    constexpr Vector2T<T> operator*(T other) const noexcept {
        return Vector2T(x * other, y * other);
    }

    constexpr Vector2T<T> operator/(T other) const {
        return Vector2T(x / other, y / other);
    }

    constexpr Vector2T<T>& operator=(Vector2T<T> const& other) noexcept {
        x = other.x;
        y = other.y;
        return *this;
    }
    constexpr bool operator==(Vector2T<T> const& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, Vector2T<T> v) {
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}
template<typename T>
struct std::hash<Vector2T<T>> {
    std::size_t operator()(Vector2T<T> v) const {
        std::hash<T> hs;
        return hs(v.x) ^ hs(v.y);
    }
};
using Vector2f = Vector2T<float>;
using Vector2d = Vector2T<double>;