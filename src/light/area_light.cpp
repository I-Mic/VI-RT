#include "light/area_light.hpp"

#include <cmath>

area_light_t::area_light_t(
    rgb_t<float> const& power,
    triangle_t const& geom
) noexcept :
    light_t{light_type_t::AREA_LIGHT},
    pdf{1.f / geom.area()},
    power{power},
    intensity{power * this->pdf},
    geom{geom} {}

light_properties_t area_light_t::get_properties(light_parameters_t const& params) const noexcept {

    vec3_t point {};
    float pdf {this->pdf};

    if(params.rand_pair.has_value()){

        float const sqrt_r0 {std::sqrt(params.rand_pair.value()[0])};
        float const alpha   {1.f - sqrt_r0};
        float const beta    {(1.f - params.rand_pair.value()[1]) * sqrt_r0};
        float const gamma   {params.rand_pair.value()[1] * sqrt_r0};

        point = alpha * this->geom.v1 + beta * this->geom.v2 + gamma * this->geom.v3;

        //if(params.point.has_value() && params.wi.has_value()){
        //    float const distance {(point - params.point.value()).norm()};
        //    pdf *= distance * distance / std::abs(params.wi.value().dot_product(this->geom.normal));
        //}
    }

    return {
        .radiance{std::make_optional(this->intensity)},
        .point{std::make_optional(point)},
        .pdf{std::make_optional(pdf)},
        .light_geom{&(this->geom)},
        .power{std::make_optional(this->power)}
    };
}
