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

    rgb::rgb_t<float> const itensity;
    rgb::rgb_t<float> const power;
    prim::geo::triangle_t gem;
    float pdf;

public:

    area_light_t(rgb::rgb_t<float> const& color) noexcept;

    ~area_light_t() noexcept;

    rgb::rgb_t<float> radiance(vec::vec3_t const&) const noexcept override;

    std::tuple<vec::vec3_t, rgb::rgb_t<float>, float> sample_l(
        float const lower, float const upper
    ) const noexcept override;

    float pdf(vec::vec3_t const& p) const noexcept override;
};

};

#endif /* AREA_LIGHT_HPP */
