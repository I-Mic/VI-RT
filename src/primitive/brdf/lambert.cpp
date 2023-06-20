#include "primitive/brdf/lambert.hpp"

rgb_t<float> lambert_t::eval_diffuse(brdf_data_t const& data) const noexcept {
    vec3_t const& wi {data.wi.value()};
    vec3_t const& sn {data.sn.value()};
    rgb_t<float> const& kd {data.material->kd};

    float const sn_dot_wi {sn.dot(wi)};
    //return kd * INVERSE_PI * sn_dot_wi;
    return kd * sn_dot_wi;
}

float lambert_t::diffuse_term(brdf_data_t const&) const noexcept {
    return 1.f;
}

