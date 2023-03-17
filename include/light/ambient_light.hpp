//
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "light/light.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

namespace light {

class ambient_light_t : public light_t {

public:

    rgb::rgb_t<float> const color;

    ambient_light_t(rgb::rgb_t<float> const& color) noexcept;

    ~ambient_light_t() noexcept;

    rgb::rgb_t<float> compute_radiance(vec::vec3_t const&) const noexcept override;

    std::tuple<vec::vec3_t, rgb::rgb_t<float>> sample_l(
        float const lower, float const upper
    ) const noexcept override;

    float pdf(vec::vec3_t const& p) const noexcept override;
};

};

#endif /* AMBIENT_LIGHT_HPP */
