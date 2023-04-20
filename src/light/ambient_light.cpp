#include "light/ambient_light.hpp"


namespace light {

ambient_light_t::ambient_light_t(rgb::rgb_t<float> const& color) noexcept : 
    light_t{light::light_type_t::AMBIENT_LIGHT},
    color{color} {}

ambient_light_t::~ambient_light_t() noexcept {}

rgb::rgb_t<float> ambient_light_t::radiance(vec::vec3_t const&) const noexcept {
    return this->color;
}

std::tuple<vec::vec3_t, rgb::rgb_t<float>> ambient_light_t::sample_l(
    float const lower, float const upper
) const noexcept {
    return std::make_tuple<vec::vec3_t, rgb::rgb_t<float>>({}, {});
}

float ambient_light_t::pdf(vec::vec3_t const& p) const noexcept {
    return 0.f;
}

};
