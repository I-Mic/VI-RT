//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "utils/rgb.hpp"
#include "utils/vector.hpp"

#include <optional>

struct intersection_t {

public:
    vec3_t p;
    vec3_t gn;  // geometric normal
    vec3_t sn;  // shading normal (the same as gn for the time being)
    vec3_t wo;
    float depth;
    std::optional<size_t> material_index;
    std::optional<rgb_t<float>> le; // has value if intersects an area light

    intersection_t() noexcept = default;
    // from pbrt book, section 2.10, pag 116
    intersection_t(
        vec3_t const& p,
        vec3_t const& n,
        vec3_t const& wo,
        float const depth
    ) noexcept :
        p{p}, gn{n}, sn{n}, wo{wo}, depth{depth},
        material_index{std::nullopt}, le{std::nullopt} {}
};

#endif /* INTERSECTION_HPP */
