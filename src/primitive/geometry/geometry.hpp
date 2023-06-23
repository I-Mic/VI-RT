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

class geometry_t {

protected:
    // geometric primitive bounding box
    bb_t const bb;

public:

    geometry_t() noexcept = default;
    geometry_t(bb_t const& bb) noexcept : bb{bb} {}

    virtual ~geometry_t() noexcept = default;

    virtual std::optional<intersection_t> intersect(ray_t const& r) const noexcept = 0;
};

#endif /* GEOMETRY_HPP */
