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


class perspective_t : public camera_t {

private:
    vec3_t const eye;
    vec3_t const at;
    vec3_t const up;
    float const fov_w;
    float const fov_h;
    std::array<std::array<float, 3>, 3> mutable cam_to_world_matrix;

public:
    perspective_t(
        vec3_t const& eye, vec3_t const& at, vec3_t const& up,
        size_t const w, size_t const h, float const fov_w, float const fov_h
    ) noexcept;

    ~perspective_t() noexcept = default;

    ray_t generate_ray(
        size_t const x, size_t const y,
        std::array<float, 2> const jitter = {0.5, 0.5}
    ) const noexcept override;
};

#endif /* PERSPECTIVE_HPP */
