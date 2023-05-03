#include "light/point_light.hpp"
#include "utils/vector.hpp"


namespace light {

point_light_t::point_light_t(rgb::rgb_t<float> const& color, vec::vec3_t const& pos) noexcept :
    light_t{light::light_type_t::POINT_LIGHT},
    color{color},
    position{pos} {}

point_light_t::~point_light_t() noexcept {}

light_properties_t point_light_t::get_properties(light_parameters_t const&) const noexcept {
    return {
		.radiance{std::make_optional(this->color)},
		.point{std::make_optional(this->position)},
	};
}

};
