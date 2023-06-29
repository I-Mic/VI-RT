//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef RGB_HPP
#define RGB_HPP

#include <type_traits>
#include <array>

template<
    typename T,
    typename = std::enable_if<
        std::is_same<T, float>::value || std::is_same<T, unsigned char>::value
    >
>
struct rgb_t {

public:

    T r, g, b;

    rgb_t() noexcept :
        r{static_cast<T>(0)},
        g{static_cast<T>(0)},
        b{static_cast<T>(0)} {}

    rgb_t(T const r, T const g, T const b) noexcept :
        r{r}, g{g}, b{b} {}

    static rgb_t from_array(std::array<T, 3> const& arr){
        return {arr[0], arr[1], arr[2]};
    }

    friend rgb_t& operator/=(rgb_t& lhs, float const rhs) noexcept {

        static_assert(std::is_same<T, float>::value);

        lhs = lhs / rhs;
        return lhs;
    }

    friend rgb_t operator/(rgb_t const& lhs, rgb_t const& rhs) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            lhs.r / rhs.r,
            lhs.g / rhs.g,
            lhs.b / rhs.b
        };
    }

    friend rgb_t operator*(rgb_t const& lhs, rgb_t const& rhs) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            lhs.r * rhs.r,
            lhs.g * rhs.g,
            lhs.b * rhs.b
        };
    }

    friend rgb_t operator*(float const f, rgb_t const& rhs) noexcept {
        return rhs * f;
    }

    friend rgb_t operator*(rgb_t const& lhs, float const f) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            lhs.r * f,
            lhs.g * f,
            lhs.b * f
        };
    }

    friend rgb_t operator/(rgb_t const& lhs, float const f) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            lhs.r / f,
            lhs.g / f,
            lhs.b / f
        };
    }

    rgb_t& operator+=(rgb_t const& rhs) noexcept {
        this->r += rhs.r;
        this->g += rhs.g;
        this->b += rhs.b;

        return *this;
    }

    rgb_t operator+(rgb_t const& rhs) const noexcept {
        return {
            this->r + rhs.r,
            this->g + rhs.g,
            this->b + rhs.b
        };
    }

    rgb_t operator-(rgb_t const& rhs) const noexcept {
        return {
            this->r - rhs.r,
            this->g - rhs.g,
            this->b - rhs.b
        };
    }

    bool is_zero() const noexcept {
        return
            this->r == static_cast<T>(0) &&
            this->g == static_cast<T>(0) &&
            this->b == static_cast<T>(0);
    }

    float luminance() const noexcept {
        return this->r * 0.2126 + this->g * 0.7152 + this->b * 0.0722;
    }
};

#endif /* RGB_HPP */
