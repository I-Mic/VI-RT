#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "utils/rgb.hpp"

struct material_t {

public:
    rgb_t<float> ka;
    rgb_t<float> kd;
    rgb_t<float> ks;
    rgb_t<float> kt;
    float ns;
};

#endif //MATERIAL_HPP
