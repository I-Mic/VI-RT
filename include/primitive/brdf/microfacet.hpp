#ifndef MICROFACET_HPP
#define MICROFACET_HPP

#include "utils/rgb.hpp"
#include "primitive/brdf/brdf.hpp"
#include "utils/vector.hpp"
#include <cmath>

/**
 * D Term - A microfacet distribution function - tells us what fraction of microfacets are oriented
 in direction H so that light incoming from direction L will be reflected in direction V.
 * F Term - Fresnel term, evaluates how much light is reflected off the surface under given angle
 of incidence.
 * G Term - Geometric attenuation term (also masking and shadowing term denoted G_2 later in
 this text), accounts for mutual shadowing (and masking) of microfacets, sometimes also used
 for normalization of BRDF.
 */

class microfacet_t : public specular_brdf_t {

private:
    // D term of microfacet model (GGX)
    float ndf(float const roughness, float const n_dot_h) const noexcept;

    // G1 term of microfacet model (using GGX lambda and Smith)
    float geo_attenuation1(float const roughness, float h_dot_wx) const noexcept;

    // G2 term of microfacet model (takes in account masking and shadowing effects)
    float geo_attenuation2(
        float const roughness,
        vec3_t const& wi,
        vec3_t const& wo,
        vec3_t const& h   //microfacet normal, not half vector!
    ) const noexcept;

    // F term of microfacet model (using Schlick's approximation)
    rgb_t<float> fresnel_schlick(rgb_t<float> const& f0, float const h_dot_wx) const noexcept;

public:

    microfacet_t() noexcept  = default;
    ~microfacet_t() noexcept = default;

    rgb_t<float> eval_specular(brdf_data_t const& data) const noexcept override;
    std::tuple<vec3_t, float> sample_specular(brdf_data_t const& data) const noexcept override;
};

#endif /* MICROFACET_HPP */
