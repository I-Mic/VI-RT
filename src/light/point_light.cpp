#include "light/point_light.hpp"
#include "utils/vector.hpp"


namespace light {

point_light_t::point_light_t(rgb::rgb_t<float> const& color, vec::vec3_t const& pos) noexcept : 
    light_t{light::light_type_t::POINT_LIGHT},
    color{color},
	position{pos} {}

point_light_t::~point_light_t() noexcept {}

rgb::rgb_t<float> point_light_t::radiance(vec::vec3_t const&) const noexcept {
    return this->color;
}

std::tuple<vec::vec3_t, rgb::rgb_t<float>,float> point_light_t::sample_l(
    float const lower, float const upper
) const noexcept {
    return std::make_tuple(this->position, this->color,0);
}

float point_light_t::pdf(vec::vec3_t const& p) const noexcept {
    return 0.f;
}

};
