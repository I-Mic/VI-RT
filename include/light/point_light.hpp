#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light/light.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

namespace light {

class point_light_t : public light_t {

private:

    rgb::rgb_t<float> const color;
    vec::vec3_t const position;

public:

    point_light_t(rgb::rgb_t<float> const& color, vec::vec3_t const& pos) noexcept;
    ~point_light_t() noexcept;

    light_properties_t get_properties(light_parameters_t const&) const noexcept override;
};

};

#endif /* POINT_LIGHT_HPP */
