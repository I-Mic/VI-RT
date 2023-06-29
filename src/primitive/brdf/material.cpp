#include "primitive/brdf/material.hpp"
#include "utils/math_extra.hpp"

rgb_t<float> const material_t::RGB_MIN_DIELECTRICS_F0 {0.04f, 0.04f, 0.04f};

material_t const material_t::DEFAULT {
    {1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f},
    1.f,
    0.f
};

material_t::material_t(
    rgb_t<float> const& ka, rgb_t<float> const& kd,
    rgb_t<float> const& ks, rgb_t<float> const& kt,
    float const ns, float const metalness
) noexcept :
    ka{ka}, kd{kd}, ks{ks}, kt{kt}, ns{ns},
    base_color{kd}, metalness{metalness},
    roughness{std::pow(2.f / (ns + 2.f), 0.25f)},
    specular_f0{emath::mix(RGB_MIN_DIELECTRICS_F0, this->base_color, metalness)},
    diffuse_reflectance{this->base_color * (1.f - metalness)} {}
