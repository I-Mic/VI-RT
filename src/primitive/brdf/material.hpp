#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>

#include "utils/rgb.hpp"

struct material_t {

public:
    static rgb_t<float> const RGB_MIN_DIELECTRICS_F0;

    rgb_t<float> ka;
    rgb_t<float> kd;
    rgb_t<float> ks;
    rgb_t<float> kt;
    float ns;

    rgb_t<float> base_color;
    float metalness;
    float roughness;
    rgb_t<float> specular_f0;
    rgb_t<float> diffuse_reflectance;

    material_t(
        rgb_t<float> const& ka, rgb_t<float> const& kd,
        rgb_t<float> const& ks, rgb_t<float> const& kt,
        float const ns, float const metalness
    ) noexcept;
};

#endif //MATERIAL_HPP
