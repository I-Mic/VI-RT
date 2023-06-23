//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <memory>

#include "primitive/geometry/geometry.hpp"

struct primitive_t {

    std::unique_ptr<geometry_t> geo;
    size_t const material_index;

    primitive_t(std::unique_ptr<geometry_t> geo, size_t const material_index) noexcept :
        geo{std::move(geo)},
        material_index{material_index} {}
};

#endif /* PRIMITIVE_HPP */
