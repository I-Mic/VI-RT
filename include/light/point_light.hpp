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

    rgb::rgb_t<float> radiance(vec::vec3_t const&) const noexcept override;

    std::tuple<vec::vec3_t, rgb::rgb_t<float>, float> sample_l(
        float const lower, float const upper
    ) const noexcept override;

    float pdf(vec::vec3_t const& p) const noexcept override;
};

};

#endif /* POINT_LIGHT_HPP */
