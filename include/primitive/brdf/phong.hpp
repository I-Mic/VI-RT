//
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#ifndef PHONG_HPP
#define PHONG_HPP

#include "utils/rgb.hpp"
#include "primitive/brdf/brdf.hpp"

namespace prim::brdf {

struct phong_t : public brdf_t {

private:
    rgb::rgb_t ka, kd, ks, kt;

public:

    phong_t(rgb::rgb_t const ka, rgb::rgb_t const kd, rgb::rgb_t const ks, rgb::rgb_t const kt); 

    ~phong_t() override;

    rgb::rgb_t f(vec::vec3_t const wi, vec::vec3_t const wo, BRDF_TYPES const type) override;

    rgb::rgb_t sample_f(
        vec::vec3_t const wi,
        float* prob,
        vec::vec3_t* wo,
        BRDF_TYPES const type
    ) override;

    float pdf(vec::vec3_t const wi, vec::vec3_t const wo, BRDF_TYPES const type) override;
};

};

#endif /* PHONG_HPP */
