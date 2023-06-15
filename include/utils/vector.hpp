//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <array>
#include <tuple>

struct vec3_t {

public:

    float x, y, z;

    vec3_t() noexcept : x{0.f}, y{0.f}, z{0.f} {}

    vec3_t(float const x, float const y, float const z) noexcept : x{x}, y{y}, z{z} {}

    vec3_t(vec3_t const& v) noexcept : x{v.x}, y{v.y}, z{v.z} {}

    vec3_t& operator=(vec3_t const& v) noexcept = default;
    vec3_t& operator=(vec3_t&& v) noexcept = default;

    static vec3_t from_array(std::array<float, 3> const& arr) noexcept {
        return {arr[0], arr[1], arr[2]};
    }

    static vec3_t surface_normal(vec3_t const& v1, vec3_t const& v2, vec3_t const& v3) noexcept {
        vec3_t const edge1 {v2 - v1};
        vec3_t const edge2 {v3 - v1};
        vec3_t normal {edge1.cross_product(edge2)};
        normal.normalize();
        return normal;
    }

    vec3_t operator+(vec3_t const& rhs) const noexcept {
        return {
            this->x + rhs.x,
            this->y + rhs.y,
            this->z + rhs.z
        };
    }

    vec3_t operator-(vec3_t const& rhs) const noexcept {
        return {
            this->x - rhs.x,
            this->y - rhs.y,
            this->z - rhs.z
        };
    }

    vec3_t& operator+=(vec3_t const& rhs) noexcept {

        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;

        return *this;
    }

    template<std::floating_point T>
    vec3_t operator*(T const rhs) const noexcept {

        float const f {static_cast<float>(rhs)};
        return {
            this->x * f,
            this->y * f,
            this->z * f
        };
    }

    template<std::floating_point T>
    vec3_t operator/(T const rhs) const noexcept {

        float const f {static_cast<float>(rhs)};
        return {
            this->x / f,
            this->y / f,
            this->z / f
        };
    }

    template<std::floating_point T>
    vec3_t& operator*=(T const rhs) noexcept {

        float const f {static_cast<float>(rhs)};
        this->x *= f;
        this->y *= f;
        this->z *= f;

        return *this;
    }

    template<std::floating_point T>
    friend vec3_t operator*(T const lhs, vec3_t const& rhs) noexcept {
        return rhs * lhs;
    }


    float norm() const noexcept {
        return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    void normalize() noexcept {

        float const norm {this->norm()};

        if (norm > 0.f) {
            this->x /= norm;
            this->y /= norm;
            this->z /= norm;
        }
    }

    float dot_product(vec3_t const& v) const noexcept {
        return this->x * v.x +
               this->y * v.y +
               this->z * v.z;
    }

    vec3_t cross_product(vec3_t const& v) const noexcept {

        return {
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x
        };
    }

    vec3_t abs() const noexcept {
        return {
            std::abs(this->x),
            std::abs(this->y),
            std::abs(this->z)
        };
    }

    size_t get_max_dimension() const noexcept {
        return
            (this->x > this->y)
                ? ((this->x > this->z) ? 0 : 2)
                : ((this->y > this->z) ? 1 : 2);
    }

    // flip a vector such that it points to the same "side" as v (positive cosine)
    // based on pbrt book, sec 2.4, pag 72
    vec3_t flip(vec3_t const& v) const noexcept {
        return this->dot_product(v) < 0.f ? (-1.f * *this) : *this;
    }

    std::tuple<vec3_t, vec3_t> coordinate_system() const noexcept {
        vec3_t v2 {};

        if(std::abs(this->x) > std::abs(this->y))
            v2 =
                vec3_t{-this->z, 0.f, this->x} /
                std::sqrt(this->x * this->x + this->z * this->z);
        else
            v2 =
                vec3_t{0.f, this->z, -this->y} /
                std::sqrt(this->y * this->y + this->z * this->z);

        vec3_t const v3 {this->cross_product(v2)};

        return std::make_tuple(v2, v3);
    }

    vec3_t rotate(vec3_t const& rx, vec3_t const& ry, vec3_t const& rz) const noexcept {
        return {
            this->x * rx.x + this->y * ry.x + this->z * rz.x,
            this->x * rx.y + this->y * ry.y + this->z * rz.y,
            this->x * rx.z + this->y * ry.z + this->z * rz.z
        };
    }
};

#endif /* VECTOR_HPP */
