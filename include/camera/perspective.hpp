//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

#include <optional>
#include <array>

#include "camera/camera.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"


namespace cam {

class perspective_t : public camera_t {

private:
    vec::vec3_t const eye;
    vec::vec3_t const at;
    vec::vec3_t const up;
    float const fov_w;
    float const fov_h;
    std::array<std::array<float, 3>, 3> c2w;

public:
    perspective_t(
        vec::vec3_t const& eye, vec::vec3_t const& at, vec::vec3_t const& up,
        size_t const w, size_t const h, float const fov_w, float const fov_h
    ) noexcept;

    std::optional<ray::ray_t> generate_ray(
        int const x, int const y,
        float const* const cam_jitter=nullptr
    ) const noexcept override;
};

};

#endif /* PERSPECTIVE_HPP */
