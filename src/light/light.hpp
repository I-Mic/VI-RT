//
//  VI-RT
//
//  Created by Luis Paulo Santos on 04/02/2023.
//

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <optional>
#include <tuple>

#include "primitive/geometry/triangle.hpp"
#include "utils/vector.hpp"
#include "utils/rgb.hpp"

enum class light_type_t {
    AMBIENT_LIGHT,
    POINT_LIGHT,
    AREA_LIGHT
};

struct light_parameters_t {
    std::optional<std::array<float, 2>> rand_pair;
};

struct light_properties_t {
    std::optional<rgb_t<float>> radiance;
    std::optional<vec3_t> point;
    std::optional<float> pdf;
    triangle_t const* light_geom {nullptr};
    std::optional<rgb_t<float>> power;
};

class light_t {

public:

    light_type_t const type;

    light_t(light_type_t const type) noexcept : type{type} {}
    virtual ~light_t() = default;

    virtual light_properties_t get_properties(
        light_parameters_t const& params = {}
    ) const noexcept = 0;
};

#endif /* LIGHT_HPP */
