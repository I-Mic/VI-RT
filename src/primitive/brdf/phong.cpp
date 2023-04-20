//
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#include <tuple>

#include "primitive/brdf/phong.hpp"

namespace prim::brdf {

phong_t::phong_t(
    rgb::rgb_t<float> const& ka,
    rgb::rgb_t<float> const& kd,
    rgb::rgb_t<float> const& ks,
    rgb::rgb_t<float> const& kt
) noexcept :
    ka{ka}, kd{kd},
    ks{ks}, kt{kt} {}

phong_t::~phong_t() noexcept {}

rgb::rgb_t<float> phong_t::compute_radiance(
    vec::vec3_t const& wi, vec::vec3_t const& wo, brdf_types_t const type
) const noexcept {
    return {};
}

std::tuple<vec::vec3_t, rgb::rgb_t<float>> phong_t::sample_f(
    vec::vec3_t const& wi, float const lower, float const upper, brdf_types_t const type
) const noexcept {
    return std::make_tuple<vec::vec3_t, rgb::rgb_t<float>>({}, {});
}

float phong_t::pdf(
    vec::vec3_t const& wi, vec::vec3_t const& wo, brdf_types_t const type
) const noexcept {
    return 0.f;
}

rgb::rgb_t<float> phong_t::ambient(rgb::rgb_t<float> const& radiance) const noexcept {
    return this->ka * radiance;
}

rgb::rgb_t<float> phong_t::diffuse(rgb::rgb_t<float> const& radiance, float cosl) const noexcept {
	return this->kd * radiance * cosl;
}


bool phong_t::is_specular_zero() const noexcept {
	return this->ks.is_zero();
}

};
