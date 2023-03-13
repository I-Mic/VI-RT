//
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/brdf.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

namespace prim::brdf {

phong_t::phong_t(
    rgb::rgb_t const ka,
    rgb::rgb_t const kd,
    rgb::rgb_t const ks,
    rgb::rgb_t const kt) : ka(ka), kd(kd), ks(ks), kt(kt) {}

phong_t::~phong_t(){}

rgb::rgb_t phong_t::f(vec::vec3_t const wi, vec::vec3_t const wo, BRDF_TYPES const type){}

rgb::rgb_t phong_t::sample_f(
    vec::vec3_t const wi,
    float* prob,
    vec::vec3_t* wo,
    BRDF_TYPES const type)
{

}

float phong_t::pdf(vec::vec3_t const wi, vec::vec3_t const wo, BRDF_TYPES const type){}

};
