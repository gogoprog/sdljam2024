#pragma once

#include <array>
#include <cmath>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <typeinfo>

using String = std::string;
template <typename T> using Vector = std::vector<T>;
/* template <typename T> using Span = std::span<T>; */
template <typename T, int size> using Array = std::array<T, size>;
template <typename T> using UniquePtr = std::unique_ptr<T>;
template <typename T> using SharedPtr = std::shared_ptr<T>;
template <typename K, typename V> using Map = std::map<K, V>;
template <typename K, typename V> using UnorderedMap = std::unordered_map<K, V>;

using TypeIndex = std::type_index;

#if __has_include(<numbers>)
#include <numbers>
#else
namespace std {
namespace numbers {
static const double pi = 3.14159265;
}
} // namespace std
#endif

class Vector2 {
  public:
    Vector2() = default;
    constexpr Vector2(int _x, int _y) : x(_x), y(_y) {
    }
    Vector2(float _x, float _y) : x(_x), y(_y) {
    }
    Vector2(double _x, double _y) : x(_x), y(_y) {
    }

    float x;
    float y;

    float getLength() const {
        return std::sqrt(x * x + y * y);
    }

    float getSquareLength() const {
        return x * x + y * y;
    }

    Vector2 getNormalized() const {
        auto len = getLength();
        return {x / len, y / len};
    }
};

inline Vector2 operator+(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

inline Vector2 operator-(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

inline Vector2 operator*(const Vector2 &v1, const float v) {
    return {v1.x * v, v1.y * v};
}

inline Vector2 operator/(const Vector2 &v1, const float v) {
    return {v1.x / v, v1.y / v};
}

inline bool operator<(const Vector2 &v1, const Vector2 &v2) {
    if (v1.y == v2.y)
        return v1.x < v2.x;
    return v1.y < v2.y;
}

inline bool operator==(const Vector2 &v1, const Vector2 &v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

inline float clampAngle(const float input) {
    float result = input;

    while (result < 0) {
        result += 360;
    }
    while (result > 360) {
        result -= 360;
    }

    return result;
}

static constexpr Array<Vector2, 8> directions = {Vector2{0, -1}, {1, 0},  {0, 1},   {-1, 0},
                                                 {1, 1},         {1, -1}, {-1, -1}, {-1, 1}};

template <>
struct std::hash<Vector2>
{
  inline std::size_t operator()(const Vector2& k) const
  {
      return k.x * 1000 + k.y;
  }
};
