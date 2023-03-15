//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BRDF_HPP
#define BRDF_HPP

#include "utils/vector.hpp"
#include "utils/rgb.hpp"


namespace prim::brdf {

enum class BRDF_TYPES {
    SPECULAR_REF   = 0b1,
    DIFFUSE_REF    = 0b10,
    SPECULAR_TRANS = 0b100,
    GLOSSY_REF     = 0b1000,
    BRDF_ALL       = 0b1111
};

struct brdf_t {

public:
    brdf_t() noexcept = default;
    virtual ~brdf_t() = default;

    // return the brdf_t RGB value for a pair of (incident, scattering) directions : (wi,wo)
    virtual rgb::rgb_t<float> compute_radiance(
        vec::vec3_t const& wi,
        vec::vec3_t const& wo,
        BRDF_TYPES const type = BRDF_TYPES::BRDF_ALL
    ) const noexcept = 0;

    // return an outgoing direction wo and brdf
    // RGB value for a given wi and probability pair prob[2]
    virtual std::tuple<vec::vec3_t, rgb::rgb_t<float>> sample_f(
        vec::vec3_t const& wi,
        float const lower, float const upper,
        BRDF_TYPES const type = BRDF_TYPES::BRDF_ALL
    ) const noexcept = 0;

    // return the probability of sampling wo given wi
    virtual float pdf(
        vec::vec3_t const& wi,
        vec::vec3_t const& wo,
        BRDF_TYPES const type = BRDF_TYPES::BRDF_ALL
    ) const noexcept = 0;
};

};


#endif /* BRDF_HPP */
