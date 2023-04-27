#include "light/area_light.hpp"


namespace light {

    area_light_t::area_light_t(rgb::rgb_t<float> const& color) noexcept :
        light_t{ light::light_type_t::AREA_LIGHT },
        color{ color } {}

    area_light_t::~area_light_t() noexcept {}

    rgb::rgb_t<float> area_light_t::radiance(vec::vec3_t const&) const noexcept {
        return this->color;
    }

    std::tuple<vec::vec3_t, rgb::rgb_t<float>,float > area_light_t::sample_l(
        float const lower, float const upper
    ) const noexcept {
        const float sqrt_r0 = sqrtf(lower);
        const float alpha = 1.f - sqrt_r0;
        const float beta = (1.f - upper) * sqrt_r0;
        const float gamma = upper * sqrt_r0;
        vec::vec3_t p;

        p.x = alpha * gem.v1.x + beta * gem.v2.x + gamma * gem.v3.x;
        p.y = alpha * gem.v1.y + beta * gem.v2.y + gamma * gem.v3.y;
        p.z = alpha * gem.v1.z + beta * gem.v2.z + gamma * gem.v3.z;

        return std::make_tuple(p,intensity,pdf);
    }

    float area_light_t::pdf(vec::vec3_t const& p) const noexcept {
        return 0.f;
    }

};
