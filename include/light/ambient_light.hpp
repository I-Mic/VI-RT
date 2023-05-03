//
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "light/light.hpp"
#include "utils/rgb.hpp"

namespace light {

class ambient_light_t : public light_t {

private:

    rgb::rgb_t<float> const color;

public:

    ambient_light_t(rgb::rgb_t<float> const& color) noexcept;
    ~ambient_light_t() noexcept;

    light_properties_t get_properties(light_parameters_t const&) const noexcept override;
};

};

#endif /* AMBIENT_LIGHT_HPP */
