//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef RGB_HPP
#define RGB_HPP

#include <type_traits>
namespace rgb {

template<typename T>
class rgb_t {

    static_assert(std::is_same<T, float>::value || std::is_same<T, unsigned char>::value);

public:

    T r, g, b;

    rgb_t() noexcept : r(0.f), g(0.f), b(0.f) {}

    rgb_t(T const r, T const g, T const b) noexcept : r(r), g(g), b(b) {}

    rgb_t(rgb_t const& rgb) noexcept : r(rgb.r), g(rgb.g), b(rgb.b) {}

    rgb_t& operator=(rgb_t const& rgb) = default;
    rgb_t& operator=(rgb_t&& rgb) = default;


    rgb_t& operator+=(rgb_t const& rhs) noexcept {

        this->r += rhs.r;
        this->g += rhs.g;
        this->b += rhs.b;

        return *this;
    }

    //float Y() { what?
    //    return (r*0.2126 + g*0.7152 + b*0.0722 );
    //}
};

};

#endif /* RGB_HPP */
