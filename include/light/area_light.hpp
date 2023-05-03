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

namespace light {

class area_light_t : public light_t {

private:

    float const pdf;
    rgb::rgb_t<float> const power;
    rgb::rgb_t<float> const intensity;
    prim::geo::triangle_t const geom;

public:

    area_light_t(
        rgb::rgb_t<float> const& power,
        prim::geo::triangle_t const& geom
    ) noexcept;

    ~area_light_t() noexcept;

    light_properties_t get_properties(light_parameters_t const&) const noexcept override;
};

};

#endif /* AREA_LIGHT_HPP */
