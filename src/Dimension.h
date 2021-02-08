#ifndef MINICRAFTSERVER_DIMENSION_H
#define MINICRAFTSERVER_DIMENSION_H

#include <cstdint>
#include <cmath>
#include <array>
#include <iostream>
#include <type_traits>

#include "MinicraftDef.h"

namespace mcplus {

    template<typename N>
    struct Vector;
    template<typename N>
    struct Location;
    template<typename N>
    struct Segment;
    template<typename N>
    struct Rectangle;
    template<typename N>
    struct Circle;

    using Vector2f    = Vector<float>;
    using Location2f  = Location<float>;
    using Segment2f   = Segment<float>;
    using Rectangle2f = Rectangle<float>;
    using Circle2f    = Circle<float>;

    using Vector2i    = Vector<int>;

    enum class Direction {
        NONE,
        DOWN,
        UP,
        LEFT,
        RIGHT
    };

    template<typename N, typename M>
    inline double distance(const Vector<N>& vec1, const Vector<M>& vec2);
    template<typename N, typename M>
    inline double distance(const Location<N>& loc1, const Location<M>& loc2);
    template<typename N, typename M>
    inline double distance(const Segment<N>& seg1, const Segment<M>& seg2);

    template<typename N>
    struct Vector {
        N x;
        N y;

        Vector(N x, N y) : x(x), y(y) {}
        Vector() : Vector(0, 0) {}

        template<typename M>
        Vector<N> operator+(const Vector<M>& vector) const {
            return Vector<N>(this->x + vector.x, this->y + vector.y);
        }

        template<typename M>
        Vector<N>& operator+=(const Vector<M>& vector) {
            this->x += vector.x;
            this->y += vector.y;
            return *this;
        }

        template<typename M>
        Vector<N> operator-(const Vector<M>& vector) const {
            return Vector<N>(this->x - vector.x, this->y - vector.y);
        }

        template<typename M>
        Vector<N>& operator-=(const Vector<M>& vector) {
            this->x -= vector.x;
            this->y -= vector.y;
            return *this;
        }

        template<typename M>
        Vector<N> operator*(const Vector<M>& vector) const {
            return Vector<N>(this->x * vector.x, this->y * vector.y);
        }

        template<typename M>
        Vector<N>& operator*=(const Vector<M>& vector) {
            this->x *= vector.x;
            this->y *= vector.y;
            return *this;
        }

        template<typename M>
        bool operator==(const Vector<M>& vector) const {
            return this->x == vector.x && this->y == vector.y;
        }

        template<typename M>
        bool operator<(const Vector<M>& vector) const {
            return this->x < vector.x && this->y < vector.y;
        }

        template<typename M>
        bool operator>(const Vector<M>& vector) const {
            return this->x > vector.x && this->y > vector.y;
        }

        template<typename M>
        bool operator<=(const Vector<M>& vector) const {
            return this->x <= vector.x && this->y <= vector.y;
        }

        template<typename M>
        bool operator>=(const Vector<M>& vector) const {
            return this->x >= vector.x && this->y >= vector.y;
        }

        [[nodiscard]] Location<N> toLocation(WorldId world) const {
            return Location<N>(world, x, y);
        }

        template<typename M>
        explicit operator Vector<M>() const {
            return Vector<M>(x, y);
        }
    };

    template<typename N>
    struct Location {
        WorldId world;
        N x;
        N y;
        Direction direction;

        Location(WorldId world, N x, N y, Direction direction) : world(world), x(x), y(y), direction(direction) {}
        Location(WorldId world, N x, N y) : world(world), x(x), y(y), direction(Direction::NONE) {}
        Location(WorldId world, const Vector<N>& position, Direction direction) : world(world), x(position.x), y(position.y), direction(direction) {}
        Location(WorldId world, const Vector<N>& position) : world(world), x(position.x), y(position.y), direction(Direction::NONE) {}
        Location(WorldId world, const FixedLocation& fixedLocation, Direction direction) : world(world), direction(direction) {
            if (std::is_floating_point<N>::value) {
                x = static_cast<N>(fixedLocation.x) + static_cast<N>(fixedLocation.xDecimal) / 100;
                y = static_cast<N>(fixedLocation.y) + static_cast<N>(fixedLocation.yDecimal) / 100;
            } else {
                x = fixedLocation.x;
                y = fixedLocation.y;
            }
        }
        Location() : world(0), x(0), y(0), direction(Direction::NONE) {}

        template<typename M>
        Location<N> operator+(const M& vector) const {
            return Location<N>(world, this->x + vector.x, this->y + vector.y, direction);
        }

        template<typename M>
        Location<N>& operator+=(const M& vector) {
            this->x += vector.x;
            this->y += vector.y;
            return *this;
        }

        template<typename M>
        Location<N> operator-(const M& vector) const {
            return Location<N>(world, this->x - vector.x, this->y - vector.y, direction);
        }

        template<typename M>
        Location<N>& operator-=(const M& vector) {
            this->x -= vector.x;
            this->y -= vector.y;
            return *this;
        }

        template<typename M>
        Location<N> operator*(const M& vector) const {
            return Location<N>(world, this->x * vector.x, this->y * vector.y, direction);
        }

        template<typename M>
        Location<N>& operator*=(const M& vector) {
            this->x *= vector.x;
            this->y *= vector.y;
            return *this;
        }

        template<typename M>
        bool operator==(const Location<M>& location) const {
            return this->world == location.world
                    && this->x == location.x
                    && this->y == location.y
                    && this->direction == location.direction;
        }

        template<typename M>
        explicit operator Vector<M>() const {
            return Vector<M>(x, y);
        }

        explicit operator FixedLocation() const {
            if (std::is_floating_point<N>::value) {
                float garbage = 0;

                return FixedLocation{static_cast<int32_t>(x), static_cast<uint32_t>(std::modf(x, &garbage) * 100),
                                     static_cast<int32_t>(y), static_cast<uint32_t>(std::modf(y, &garbage) * 100)};
            }

            return FixedLocation{static_cast<int32_t>(x), 0, static_cast<int32_t>(y), 0};
        }
    };

    template<typename N>
    struct Segment {
    private:
        friend Rectangle<N>;

        template<typename M>
        bool isIntercepted(std::array<Segment<M>, 4> vertices) {
            return vertices[0].isIntercepted(*this)
                   || vertices[1].isIntercepted(*this)
                   || vertices[2].isIntercepted(*this)
                   || vertices[3].isIntercepted(*this);
        }
    public:
        N sx;
        N sy;
        N ex;
        N ey;

        Segment(N sx, N sy, N ex, N ey) : sx(sx), sy(sy), ex(ex), ey(ey) {}
        Segment(const Vector<N>& startPoint, const Vector<N>& endPoint) : sx(startPoint.x), sy(startPoint.y), ex(endPoint.x), ey(endPoint.y) {}
        Segment() : sx(0), sy(0), ex(0), ey(0) {}

        Vector<N> time(float time) {
            return Vector<N>((ex - sx) * time + sx, (ey - sy) * time + sy);
        }

        template<typename M>
        bool isIntercepted(const Segment<M>& segment) const {
            N pax = ex - sx;
            N pay = ey - sy;
            N pbx = sx;
            N pby = sy;
            N qax = segment.ex - segment.sx;
            N qay = segment.ey - segment.sy;
            N qbx = segment.sx;
            N qby = segment.sy;

            float time = (pay * ((qbx - pbx) / pax) + pby - qby) / (qay - (pay * qax) / pax);
            return time >= 0 && time <= 1;
        }

        template<typename M>
        bool isIntercepted(const Vector<M>& vector) const {
            return isIntercepted(Segment<M>(vector, vector));
        }

        template<typename M>
        bool isIntercepted(const Rectangle<M>& rectangle) const {
            return isIntercepted(rectangle.vertices());
        }

        template<typename M>
        bool isIntercepted(const Circle<M>& circle) const {
            N mx = ex - sx - circle.x;
            N my = ey - sy - circle.y;
            N bx = sx - circle.x;
            N by = sy - circle.y;

            N a = mx + my;
            N b = 2 * (mx * bx + my * by);
            N c = bx * bx + by * by - circle.radius * circle.radius;

            N outSqrt = b * b - 4 * a * c;

            if (a == 0 || outSqrt < 0) {
                return false;
            }

            N resultSqrt = std::sqrt(outSqrt);
            N positiveResult = (-b + resultSqrt) / 2 * a;
            N negativeResult = (-b - resultSqrt) / 2 * a;

            return positiveResult >= 0 && positiveResult <= 1 || negativeResult >= 0 && negativeResult <= 1;
        }

        template<typename M>
        bool isInside(const Circle<M>& circle) const {
            auto center = Vector2f(circle.x, circle.y);
            return distance(Vector2f(sx, sy), center) < circle.radius
                && distance(Vector2f(ex, ey), center) < circle.radius;
        }

        template<typename M>
        bool isInside(const Rectangle<M>& rectangle) const {
            M width = rectangle.x + rectangle.width;
            M height = rectangle.y + rectangle.height;

            return rectangle.x < sx < width && rectangle.y < sy < height
                && rectangle.x < ex < width && rectangle.y < ey < height;
        }

        template<typename M>
        bool operator==(const Segment<M>& segment) const {
            return sx == segment.sx && sy == segment.sy && ex == segment.ex && ey == segment.ey;
        }
    };

    template<typename N>
    struct Rectangle {
        N x;
        N y;
        N width;
        N height;

        Rectangle(N x, N y, N width, N height) : x(x), y(y), width(width), height(height) {}
        Rectangle(const Vector<N>& point, N width, N height) : x(point.x), y(point.y), width(width), height(height) {}
        Rectangle() : x(0), y(0), width(0), height(0) {}

        template<typename M>
        bool operator==(const Rectangle<M>& rectangle) const {
            return x == rectangle.x && y == rectangle.y && width == rectangle.width && height == rectangle.height;
        }

        std::array<Segment<N>, 4> vertices() const {
            std::array<Segment<N>, 4> vertices{};

            vertices[0] = Segment<N>(x, y, x + width, y);
            vertices[1] = Segment<N>(x, y, x, y + height);
            vertices[2] = Segment<N>(x + width, y, x + width, y + height);
            vertices[3] = Segment<N>(x, y + height, x + width, y + height);

            return vertices;
        }

        template<typename M>
        bool isIntercepted(const Rectangle<M>& rectangle) const {
            auto anotherVertices = rectangle.vertices();
            auto vertices = this->vertices();

            return anotherVertices[0].isIntercepted(vertices)
                   || anotherVertices[1].isIntercepted(vertices)
                   || anotherVertices[2].isIntercepted(vertices)
                   || anotherVertices[3].isIntercepted(vertices);
        }

        template<typename M>
        bool isInside(const Rectangle<M>& rectangle) const {
            return false;
        }

        template<typename M>
        bool isInside(const Circle<M>& circle) const {
            return false;
        }

    };

    template<typename N>
    struct Circle {
        N x;
        N y;
        N radius;

        Circle(N x, N y, N radius) : x(x), y(y), radius(radius) {}
        Circle(const Vector<N>& center, N radius) : x(center.x), y(center.y), radius(radius) {}
        Circle() : x(0), y(0), radius(0) {}

        template<typename M>
        bool operator==(const Circle<M> & circle) const {
            return x == circle.x && y == circle.y && radius == circle.radius;
        }

        template<typename M>
        bool isIntercepted(const Circle<M>& circle) const {
            return false;
        }

        template<typename M>
        bool isInside(const Circle<M>& circle) const {
            return false;
        }

        template<typename M>
        bool isInsideRectangle(const Rectangle<M>& rectangle) const {
            return false;
        }
    };

    Direction operator+(const Direction& original, const Direction& direction);
    Direction& operator+=(Direction& original, const Direction& direction);
    Direction operator-(const Direction& original, const Direction& direction);
    Direction& operator-=(Direction& original, const Direction& direction);
    Direction operator*(const Direction& original, const Direction& direction);
    Direction& operator*=(Direction& original, const Direction& direction);
    Direction& operator++(Direction& direction);
    Direction& operator--(Direction& direction);

    template<typename N, typename M>
    Direction toDirection(const Vector<N>& vec1, const Vector<M>& vec2) {
        auto xd = vec2.x - vec1.x;
        auto yd = vec2.y - vec1.y;

        auto axd = std::abs(xd);
        auto ayd = std::abs(yd);

        if (axd > ayd) {
            if (xd > 0) {
                return Direction::RIGHT;
            } else if (xd < 0) {
                return Direction::LEFT;
            }
        } else if (axd < ayd) {
            if (yd > 0) {
                return Direction::UP;
            } else if (yd < 0) {
                return Direction::DOWN;
            }
        }

        return Direction::NONE;
    }

    inline double distance(double sx, double ex, double sy, double ey) {
        auto x = ex - sx;
        auto y = ey - sy;
        return std::sqrt(x * x + y * y);
    }

    template<typename N, typename M>
    inline double distance(const Vector<N>& vec1, const Vector<M>& vec2) {
        return distance(vec1.x, vec2.x, vec1.y, vec2.y);
    }

    template<typename N, typename M>
    inline double distance(const Location<N>& loc1, const Location<M>& loc2) {
        return distance(loc1.x, loc2.x, loc1.y, loc2.y);
    }

    template<typename N, typename M>
    inline double distance(const Segment<N>& seg1, const Segment<M>& seg2) {
        return distance(seg2.sx - seg2.sx, seg2.ex - seg1.ex, seg2.sy - seg2.sy, seg2.ey - seg1.ey);
    }
}

template<typename N>
std::ostream& operator<<(std::ostream& ostream, const mcplus::Vector<N>& vector) {
    ostream << "x:" <<  vector.x << " y:" << vector.y;
    return ostream;
}

template<typename N>
std::ostream& operator<<(std::ostream& ostream, const mcplus::Location<N>& location) {
    ostream << "world:" << location.world << " x:" <<  location.x << " y:" << location.y << " direction:" << location.direction;
    return ostream;
}

template<typename N>
std::ostream& operator<<(std::ostream& ostream, const mcplus::Segment<N>& segment) {
    ostream << "sx:" <<  segment.sx << " sy:" << segment.sy << " ex:" << segment.ex << " ey:" << segment.ey;
    return ostream;
}

template<typename N>
std::ostream& operator<<(std::ostream& ostream, const mcplus::Rectangle<N>& rectangle) {
    ostream << "x:" <<  rectangle.x << " y:" << rectangle.y << " width:" << rectangle.width << " height:" << rectangle.height;
    return ostream;
}

template<typename N>
std::ostream& operator<<(std::ostream& ostream, const mcplus::Circle<N>& circle) {
    ostream << "x:" <<  circle.x << " y:" << circle.y << " radius:" << circle.radius;
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const mcplus::Direction& direction);

namespace std {
    template<>
    struct [[maybe_unused]] hash<mcplus::Vector2i> {
        std::size_t operator()(const mcplus::Vector2i& vector) const noexcept {
            std::size_t h1 = std::hash<int>{}(vector.x);
            std::size_t h2 = std::hash<int>{}(vector.y);

            return h1 ^ (h2 << 1);
        }
    };
}

#endif //MINICRAFTSERVER_DIMENSION_H
