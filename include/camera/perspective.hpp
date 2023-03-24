//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

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

public:
    perspective_t(
        vec::vec3_t const& eye, vec::vec3_t const& at, vec::vec3_t const& up,
        size_t const w, size_t const h, float const fov_w, float const fov_h
    ) noexcept;

    ray::ray_t generate_ray(
        size_t const x, size_t const y,
        float const* const cam_jitter=nullptr
    ) const noexcept override;
};

};

#endif /* PERSPECTIVE_HPP */
