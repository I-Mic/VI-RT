//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef RAY_HPP
#define RAY_HPP

#include "utils/vector.hpp"

namespace ray {

struct ray_t {

public:

    vec::vec3_t org; // ray origin
    vec::vec3_t dir; // ray direction

    ray_t(){}
    ray_t(vec::vec3_t const& o, vec::vec3_t const& d): org(o), dir(d) {}
};

};

#endif /* RAY_HPP */
