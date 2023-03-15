//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <memory>

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
    std::shared_ptr<prim::brdf::brdf_t> f;
    
    
    intersection_t() noexcept {}
    // from pbrt book, section 2.10, pag 116
    intersection_t(
        vec::vec3_t const& p,
        vec::vec3_t const& n,
        vec::vec3_t const& wo,
        float const depth
    ) noexcept : p{p}, gn{n}, sn{n}, wo{wo}, depth{depth}, f{} {}
};

};

#endif /* INTERSECTION_HPP */
