//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <memory>

#include "primitive/geometry/geometry.hpp"

namespace prim {

struct primitive_t {
    std::unique_ptr<geo::geometry_t> g;
    size_t material_index;

    primitive_t(std::unique_ptr<geo::geometry_t> g, size_t const material_index);
};

};

#endif /* PRIMITIVE_HPP */
