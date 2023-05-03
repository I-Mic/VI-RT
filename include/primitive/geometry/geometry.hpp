//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <optional>

#include "primitive/bb.hpp"
#include "rays/ray.hpp"
#include "rays/intersection.hpp"

namespace prim::geo {

class geometry_t {

protected:
    // geometric primitive bounding box
    prim::bb_t const bb;

public:

    geometry_t() noexcept = default;
    geometry_t(prim::bb_t const& bb) noexcept : bb{bb} {}

    virtual ~geometry_t() = default;

    virtual std::optional<ray::intersection_t> intersect(ray::ray_t const& r) const = 0;
};

};

#endif /* GEOMETRY_HPP */
