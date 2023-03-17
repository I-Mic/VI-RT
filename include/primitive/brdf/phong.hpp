//
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#ifndef PHONG_HPP
#define PHONG_HPP

#include "utils/rgb.hpp"
#include "primitive/brdf/brdf.hpp"
#include "utils/vector.hpp"

namespace prim::brdf {

struct phong_t : public brdf_t {

public:
    rgb::rgb_t<float> const ka;
    rgb::rgb_t<float> const kd;
    rgb::rgb_t<float> const ks;
    rgb::rgb_t<float> const kt;


    phong_t(
        rgb::rgb_t<float> const& ka,
        rgb::rgb_t<float> const& kd,
        rgb::rgb_t<float> const& ks,
        rgb::rgb_t<float> const& kt
    ) noexcept;

    ~phong_t() noexcept;

    rgb::rgb_t<float> compute_radiance(
        vec::vec3_t const& wi,
        vec::vec3_t const& wo,
        BRDF_TYPES const type
    ) const noexcept override;

    std::tuple<vec::vec3_t, rgb::rgb_t<float>> sample_f(
        vec::vec3_t const& wi,
        float const lower, float const upper,
        BRDF_TYPES const type
    ) const noexcept override;

    float pdf(
        vec::vec3_t const& wi,
        vec::vec3_t const& wo,
        BRDF_TYPES const type
    ) const noexcept override;
};

};

#endif /* PHONG_HPP */
