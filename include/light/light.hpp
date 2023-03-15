//
//  VI-RT
//
//  Created by Luis Paulo Santos on 04/02/2023.
//

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <tuple>
#include "utils/vector.hpp"
#include "utils/rgb.hpp"

namespace light {

class light_t {

public:

    light_t() noexcept = default;

    virtual ~light_t() = default;

    // return the light_t RGB radiance for a given point : p
    virtual rgb::rgb_t<float> compute_radiance(vec::vec3_t const& p) const noexcept = 0;

    // return a point p and rgb::rgb_t radiance for a given probability pair prob[2]
    virtual std::tuple<vec::vec3_t, rgb::rgb_t<float>> sample_l(
        float const lower, float const upper
    ) const noexcept = 0;

    // return the probability of p
    virtual float pdf(vec::vec3_t const& p) const noexcept = 0;
};

};

#endif /* LIGHT_HPP */
