#include "utils/vector.hpp"
#include "utils/rgb.hpp"
#include "primitive/brdf/brdf.hpp"


namespace prim::brdf {

brdf_t::brdf_t(){}

brdf_t::~brdf_t(){}
//
// return the brdf_t RGB value for a pair of (incident, scattering) directions : (wi,wo)
rgb::rgb_t brdf_t::f(vec::vec3_t const wi, vec::vec3_t const wo, BRDF_TYPES const type){
    return rgb::rgb_t();
}

// return an outgoing direction wo and brdf RGB value for a given wi and probability pair prob[2]
rgb::rgb_t brdf_t::sample_f(vec::vec3_t const wi, float *prob, vec::vec3_t *wo, const BRDF_TYPES){
    return rgb::rgb_t();
}

// return the probability of sampling wo given wi
float brdf_t::pdf(vec::vec3_t wi, vec::vec3_t wo, BRDF_TYPES const type) {
    return 0.;
}

};
