#ifndef DIFFUSE_BRDF_HPP
#define DIFFUSE_BRDF_HPP

#include <optional>
#include <tuple>

#include "utils/vector.hpp"
#include "utils/rgb.hpp"
#include "primitive/brdf/material.hpp"

inline float constexpr TWO_PI {2.f * M_PI};
inline float constexpr INVERSE_PI {1.f / M_PI};
inline float constexpr INVERSE_TWO_PI {1.f / (2.f * M_PI)};

struct brdf_data_t {
    // Vectors
    std::optional<vec3_t> wo; //< Direction to viewer (or opposite direction of incident ray)
    std::optional<vec3_t> wi; //< Direction to light (or direction of reflecting ray)
    std::optional<vec3_t> sn; // Shading normal
    std::optional<vec3_t> half; //< Half vector (microfacet normal)
    std::optional<std::array<float, 2>> rand_pair;
    material_t const* material {nullptr};
};


struct diffuse_brdf_t {

public:

    diffuse_brdf_t() noexcept = default;
    virtual ~diffuse_brdf_t() = default;

    virtual rgb_t<float> eval_diffuse(brdf_data_t const& data) const noexcept = 0;
    virtual float diffuse_term(brdf_data_t const& data) const noexcept = 0;
};

struct specular_brdf_t {

public:

    specular_brdf_t() noexcept = default;
    virtual ~specular_brdf_t() = default;

    virtual rgb_t<float> eval_specular(brdf_data_t const& data) const noexcept = 0;
    //virtual std::tuple<vec3_t, float> sample_specular_half_vec(
    //    brdf_data_t const& data
    //) const noexcept = 0;
    virtual std::tuple<vec3_t, float> sample_specular(brdf_data_t const& data) const noexcept = 0;

    std::tuple<vec3_t, float> sample_hemisphere(
        std::array<float, 2> const rand_pair
    ) const noexcept {

        vec3_t const wi {
            std::cos(TWO_PI * rand_pair[0]) * std::sqrt(1.f - rand_pair[1]),
            std::sin(TWO_PI * rand_pair[0]) * std::sqrt(1.f - rand_pair[1]),
            std::sqrt(rand_pair[1])
        };

        float const cos_theta {wi.z};
        float const pdf {cos_theta * INVERSE_PI};

        return std::make_tuple(wi, pdf);
    }
};

#endif /* DIFFUSE_BRDF_HPP */
