#include "primitive/brdf/material.hpp"

rgb_t<float> const material_t::MIN_DIELECTRICS_F0 {0.04f, 0.04f, 0.04f};

material_t::material_t(
    rgb_t<float> const& ka, rgb_t<float> const& kd,
    rgb_t<float> const& ks, rgb_t<float> const& kt,
    float const ns, float const metalness
) noexcept :
    ka{ka}, kd{kd}, ks{ks}, kt{kt}, ns{ns},
    metalness{metalness},
    roughness{std::sqrt(2.f / (ns + 2.f))},
    f0{MIN_DIELECTRICS_F0 * (1.f - metalness) + kd * metalness},
    diffuse_reflectance{kd * (1.f - metalness)} {}
