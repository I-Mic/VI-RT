//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include <array>

#include "camera/perspective.hpp"
#include "utils/vector.hpp"

perspective_t::perspective_t(
    vec3_t const& eye, vec3_t const& at, vec3_t const& up,
    size_t const w, size_t const h, float const fov_w, float const fov_h
) noexcept :
    camera_t{w, h}, eye{eye}, at{at}, up{up},
    fov_w{fov_w}, fov_h{fov_h}, cam_to_world_matrix{}
{
    vec3_t const f {vec3_t::normalized(at - eye)};
    vec3_t const r {vec3_t::normalized(f.cross(up))};
    vec3_t const real_up {-1.f * vec3_t::normalized(r.cross(f))}; // flip the image

    this->cam_to_world_matrix[0] = {r.x, r.y, r.z};
    this->cam_to_world_matrix[1] = {real_up.x, real_up.y, real_up.z};
    this->cam_to_world_matrix[2] = {f.x, f.y, f.z};
}


static vec3_t matrix_product(
    std::array<std::array<float, 3>, 3> const& cam_to_world_matrix,
    std::array<float, 3> const& p
) noexcept {

    std::array<float, 3> res {0.f, 0.f, 0.f};

    for(size_t i {0}; i < 3; ++i)
        for(size_t j {0}; j < 3; ++j)
            res[i] += cam_to_world_matrix[i][j] * p[j];

    return vec3_t::from_array(res);
}

ray_t perspective_t::generate_ray(
    size_t const x, size_t const y, std::array<float, 2> const cam_jitter
) const noexcept {

    float const xs {2.f * (x + cam_jitter[0]) / this->width  - 1};
    float const ys {2.f * (y + cam_jitter[1]) / this->height - 1};

    float const xc {xs * std::tan(this->fov_w / 2.f)};
    float const yc {ys * std::tan(this->fov_h / 2.f)};

    vec3_t const ray_dir {
        vec3_t::normalized(matrix_product(this->cam_to_world_matrix, {xc, yc, 1.f}))
    };

    return {this->eye, ray_dir};
}
