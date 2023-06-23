#include "light/point_light.hpp"

point_light_t::point_light_t(rgb_t<float> const& color, vec3_t const& pos) noexcept :
    light_t{light_type_t::POINT_LIGHT},
    color{color},
    position{pos} {}

light_properties_t point_light_t::get_properties(light_parameters_t const&) const noexcept {
    return {
        .radiance{std::make_optional(this->color)},
        .point{std::make_optional(this->position)},
    };
}
