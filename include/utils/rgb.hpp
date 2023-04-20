//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef RGB_HPP
#define RGB_HPP

#include <type_traits>
#include <array>

namespace rgb {

template<typename T>
struct rgb_t {

    static_assert(std::is_same<T, float>::value || std::is_same<T, unsigned char>::value);

public:

    T r, g, b;

    rgb_t() noexcept :
        r{static_cast<T>(0)},
        g{static_cast<T>(0)},
        b{static_cast<T>(0)} {}

    rgb_t(T const r, T const g, T const b) noexcept : r{r}, g{g}, b{b} {}

    rgb_t(rgb_t const& rgb) noexcept : r{rgb.r}, g{rgb.g}, b{rgb.b} {}

    rgb_t& operator=(rgb_t const& rgb) = default;
    rgb_t& operator=(rgb_t&& rgb) = default;

	static rgb_t from_array(std::array<T, 3> const& arr){
		return {arr[0], arr[1], arr[2]};
	}


    friend rgb_t operator*(rgb_t const& lhs, rgb_t const& rhs) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            (lhs.r * rhs.r),
            (lhs.g * rhs.g),
            (lhs.b * rhs.b)
        };
    }

    friend rgb_t operator*(float const f, rgb_t const& rhs) noexcept {
		return rhs * f;
    }

    friend rgb_t operator*(rgb_t const& lhs, float const f) noexcept {

        static_assert(std::is_same<T, float>::value);

        return {
            (lhs.r * f),
            (lhs.g * f),
            (lhs.b * f)
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

	bool is_zero() const noexcept {
		return this->r == 0 && this->g == 0 && this->b == 0;
	}

    //float Y() { what?
    //    return (r*0.2126 + g*0.7152 + b*0.0722 );
    //
    //}
};

};

#endif /* RGB_HPP */
