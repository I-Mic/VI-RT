//
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef AREA_LIGHT_HPP
#define AREA_LIGHT_HPP

#include "light/light.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"
#include "primitive/geometry/triangle.hpp"

class area_light_t : public light_t {

private:

    float const pdf;
    rgb_t<float> const power;
    rgb_t<float> const intensity;
    triangle_t const geom;

public:

    area_light_t(
        rgb_t<float> const& power,
        triangle_t const& geom
    ) noexcept;

    ~area_light_t() noexcept = default;

    light_properties_t get_properties(light_parameters_t const&) const noexcept override;
};

#endif /* AREA_LIGHT_HPP */
