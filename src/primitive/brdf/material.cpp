#include "primitive/brdf/material.hpp"
#include "utils/math_extra.hpp"

rgb_t<float> const material_t::RGB_MIN_DIELECTRICS_F0 {0.04f, 0.04f, 0.04f};

material_t::material_t(
    rgb_t<float> const& ka, rgb_t<float> const& kd,
    rgb_t<float> const& ks, rgb_t<float> const& kt,
    float const ns, float const metalness
) noexcept :
    ka{ka}, kd{kd}, ks{ks}, kt{kt}, ns{ns},
    base_color{kd},                                        //FIXME
    metalness{metalness},
    roughness{std::sqrt(2.f / (ns + 2.f))},
    specular_f0{emath::lerp(RGB_MIN_DIELECTRICS_F0, this->base_color, metalness)},
    diffuse_reflectance{this->base_color * (1.f - metalness)} {}
