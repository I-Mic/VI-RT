//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "utils/vector.hpp"

namespace ray {

struct intersection_t {

public:
    vec::vec3_t p;
    vec::vec3_t gn;  // geometric normal
    vec::vec3_t sn;  // shading normal (the same as gn for the time being)
    vec::vec3_t wo;
    float depth;
    size_t material_index;
    
    
    intersection_t() noexcept {}
    // from pbrt book, section 2.10, pag 116
    intersection_t(
        vec::vec3_t const& p,
        vec::vec3_t const& n,
        vec::vec3_t const& wo,
        float const depth
    ) noexcept : p{p}, gn{n}, sn{n}, wo{wo}, depth{depth}, material_index{0} {}
};

};

#endif /* INTERSECTION_HPP */
