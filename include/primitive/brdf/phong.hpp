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

class phong_t : public brdf_t {

private:

    rgb_t<float> const ka;
    rgb_t<float> const kd;
    rgb_t<float> const ks;
    rgb_t<float> const kt;
    float const ns;

public:

    phong_t(
        rgb_t<float> const& ka,
        rgb_t<float> const& kd,
        rgb_t<float> const& ks,
        rgb_t<float> const& kt,
        float const ns
    ) noexcept;

    ~phong_t() noexcept;

    rgb_t<float> compute_radiance(
        vec3_t const& wi,
        vec3_t const& wo,
        brdf_types_t const type
    ) const noexcept override;

    std::tuple<vec3_t, rgb_t<float>> sample_f(
        vec3_t const& wi,
        float const lower, float const upper,
        brdf_types_t const type
    ) const noexcept override;

    float pdf(
        vec3_t const& wi,
        vec3_t const& wo,
        brdf_types_t const type
    ) const noexcept override;

    rgb_t<float> ambient() const noexcept override;

    rgb_t<float> diffuse() const noexcept override;

    rgb_t<float> specular() const noexcept override;

    float specular_exp() const noexcept override;
};

#endif /* PHONG_HPP */
