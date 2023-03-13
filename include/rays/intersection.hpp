//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "utils/vector.hpp"
#include "primitive/brdf/brdf.hpp"

namespace ray {

struct intersection_t {
public:
    vec::vec3_t p;
    vec::vec3_t gn;  // geometric normal
    vec::vec3_t sn;  // shading normal (the same as gn for the time being)
    vec::vec3_t wo;
    float depth;
    prim::brdf::brdf_t *f;
    
    
    intersection_t() {}
    // from pbrt book, section 2.10, pag 116
    intersection_t(const vec::vec3_t &p, const vec::vec3_t &n, const vec::vec3_t &wo, const float &depth)
    : p(p), gn(n), sn(n), wo(wo), depth(depth), f(nullptr) { }

};

};

#endif /* INTERSECTION_HPP */
