//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <memory>

#include "primitive/primitive.hpp"
#include "primitive/geometry/geometry.hpp"

namespace prim {

primitive_t::primitive_t(
    std::unique_ptr<geo::geometry_t> g,
    size_t const material_index
) noexcept :
    g{std::move(g)},
    material_index{material_index} {}

};
