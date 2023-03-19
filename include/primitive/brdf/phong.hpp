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

class phong_t : public brdf_t {

private:

    rgb::rgb_t<float> const ka;
    rgb::rgb_t<float> const kd;
    rgb::rgb_t<float> const ks;
    rgb::rgb_t<float> const kt;

public:

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
        brdf_types_t const type
    ) const noexcept override;

    std::tuple<vec::vec3_t, rgb::rgb_t<float>> sample_f(
        vec::vec3_t const& wi,
        float const lower, float const upper,
        brdf_types_t const type
    ) const noexcept override;

    float pdf(
        vec::vec3_t const& wi,
        vec::vec3_t const& wo,
        brdf_types_t const type
    ) const noexcept override;

    rgb::rgb_t<float> ambient(
        rgb::rgb_t<float> const& radiance
    ) const noexcept override;
};

};

#endif /* PHONG_HPP */
