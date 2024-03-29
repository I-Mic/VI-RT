//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_HPP
#define BB_HPP

#include "utils/vector.hpp"
#include "rays/ray.hpp"


struct bb_t {

public:
    vec3_t min;
    vec3_t max;

    bb_t() noexcept;
    bb_t(vec3_t const& p1) noexcept;
    bb_t(vec3_t const& p1, vec3_t const& p2) noexcept;
    bb_t(vec3_t const& p1, vec3_t const& p2, vec3_t const& p3) noexcept;

    static bb_t from_union_of(bb_t const& b1, bb_t const& b2) noexcept;

    bool intersects(ray_t const& r) const noexcept;
};

#endif /* BB_HPP */
