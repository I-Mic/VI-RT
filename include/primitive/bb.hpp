//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_HPP
#define BB_HPP

#include "utils/vector.hpp"
#include "rays/ray.hpp"


namespace prim {

struct bb_t {

public:
    vec::vec3_t min, max;

    bb_t();
    bb_t(vec::vec3_t const& p1);
    bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2);
    bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2, vec::vec3_t const& p3);

    static bb_t from_union_of(bb_t const& b1, bb_t const& b2);

    bool intersects(ray::ray_t const& r) const;
};

};

#endif /* BB_HPP */
