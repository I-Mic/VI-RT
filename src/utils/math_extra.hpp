#ifndef MATH_EXTRA_HPP
#define MATH_EXTRA_HPP

#include <array>
#include <random>
#include <type_traits>

namespace emath {

inline float constexpr PI {M_PI};
inline float constexpr TWO_PI {2.f * M_PI};
inline float constexpr INVERSE_PI {1.f / M_PI};
inline float constexpr INVERSE_TWO_PI {1.f / (2.f * M_PI)};
inline float constexpr EPSILON {std::numeric_limits<float>::epsilon()};

template<typename T, typename F, typename = std::enable_if<std::is_fundamental<F>::value>>
constexpr T mix(T const& t1, T const& t2, F const factor) noexcept {
    return t1 * (static_cast<F>(1) - factor) + t2 * factor;
}

inline
float rand() noexcept {
    static thread_local std::mt19937 generator {std::random_device{}()};
    static thread_local std::uniform_real_distribution<float> distr {};
    return distr(generator);
}

template<size_t size>
std::array<float, size> rand_tuple() noexcept {

    std::array<float, size> values {};
    for(float& v : values)
        v = rand();

    return values;
}

template<typename T, typename = std::enable_if<std::is_fundamental<T>::value>>
constexpr T clamp(T const value, T const min, T const max) noexcept {
    return std::max(std::min(max, value), min);
}

template<typename T, typename = std::enable_if<std::is_floating_point<T>::value>>
T degrees_to_rads(T const d) noexcept {
    static T constexpr factor {static_cast<T>(PI / 180.f)};
    return d * factor;
}

template<typename T, typename = std::enable_if<std::is_floating_point<T>::value>>
T rads_to_degrees(T const r) noexcept {
    static T constexpr factor {static_cast<T>(180.f * INVERSE_PI)};
    return r * factor;
}

}

#endif //MATH_EXTRA_HPP
