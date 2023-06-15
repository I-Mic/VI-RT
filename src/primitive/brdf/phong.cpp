//
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#include <tuple>

#include "primitive/brdf/phong.hpp"

phong_t::phong_t(
    rgb_t<float> const& ka,
    rgb_t<float> const& kd,
    rgb_t<float> const& ks,
    rgb_t<float> const& kt,
    float const ns
) noexcept :
    ka{ka}, kd{kd},
    ks{ks}, kt{kt},
    ns{ns} {}

phong_t::~phong_t() noexcept {}

rgb_t<float> phong_t::compute_radiance(
    vec3_t const& wi, vec3_t const& wo, brdf_types_t const type
) const noexcept {
    return {};
}

std::tuple<vec3_t, rgb_t<float>> phong_t::sample_f(
    vec3_t const& wi, float const lower, float const upper, brdf_types_t const type
) const noexcept {
    return std::make_tuple<vec3_t, rgb_t<float>>({}, {});
}

float phong_t::pdf(
    vec3_t const& wi, vec3_t const& wo, brdf_types_t const type
) const noexcept {
    return 0.f;
}

rgb_t<float> phong_t::ambient() const noexcept {
    return this->ka;
}

rgb_t<float> phong_t::diffuse() const noexcept {
    return this->kd;
}

rgb_t<float> phong_t::specular() const noexcept {
    return this->ks;
}

float phong_t::specular_exp() const noexcept {
    return this->ns;
}
