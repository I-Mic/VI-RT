//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

namespace vec {

class vec3_t {

public:

    float x, y, z;

    vec3_t() noexcept : x{0.f}, y{0.f}, z{0.f} {}

    vec3_t(float const x, float const y, float const z) noexcept : x{x}, y{y}, z{z} {}

    vec3_t(vec3_t const& v) noexcept : x{v.x}, y{v.y}, z{v.z} {}

    vec3_t& operator=(vec3_t const& v) noexcept = default;
    vec3_t& operator=(vec3_t&& v) noexcept = default;


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
        return std::sqrt(this->x * this->x + this->y + this->y + this->z + this->z);
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
        return
            (this->dot_product(v) < 0.f)
                ? (-1.f * *this)
                : *this;
    }
};

};


#endif /* VECTOR_HPP */
