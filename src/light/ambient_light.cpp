#include "light/ambient_light.hpp"

namespace light {

ambient_light_t::ambient_light_t(rgb::rgb_t<float> const& color) noexcept :
    light_t{light::light_type_t::AMBIENT_LIGHT},
    color{color} {}

ambient_light_t::~ambient_light_t() noexcept {}

light_properties_t ambient_light_t::get_properties(light_parameters_t const&) const noexcept {
    return {
		.radiance{std::make_optional(this->color)}
	};
}

};
