//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "camera/perspective.hpp"
#include "utils/vector.hpp"

namespace cam {

perspective_t::perspective_t(
    vec::vec3_t const& eye, vec::vec3_t const& at, vec::vec3_t const& up,
    size_t const w, size_t const h, float const fov_w, float const fov_h
) noexcept :
    camera_t{w, h}, eye{eye}, at{at}, up{up},
    fov_w{fov_w}, fov_h{fov_h}, c2w{}
{
    vec::vec3_t f {this->at - this->eye};
    f.normalize();
    vec::vec3_t r {f.cross_product(this->up)};
    r.normalize();
	vec::vec3_t real_up {r.cross_product(f)};
	real_up.normalize();

	//flip image
	//real_up *= -1.f;

    this->c2w[0] = {r.x, r.y, r.z};
    this->c2w[1] = {real_up.x, real_up.y, real_up.z};
    this->c2w[2] = {f.x, f.y, f.z};
}

static vec::vec3_t matrix_product(
    std::array<std::array<float, 3>, 3> const& c2w,
    std::array<float, 3> const& p
) noexcept {

    std::array<float, 3> res {0.f, 0.f, 0.f};

    for(size_t i {0}; i < 3; ++i)

        for(size_t j {0}; j < 3; ++j)

            res[i] += c2w[i][j] * p[j];

    return {res[0], res[1], res[2]};
}

ray::ray_t perspective_t::generate_ray(
    size_t const x, size_t const y, float const* const cam_jitter
) const noexcept {

    float const xs {2.f * (x + 0.5f) / this->width - 1};
    float const ys {2.f * (y + 0.5f) / this->height - 1};

    float const xc {xs * std::tan(this->fov_w / 2.f)};
    float const yc {ys * std::tan(this->fov_h / 2.f)};

    vec::vec3_t ray_dir {matrix_product(this->c2w, {xc, yc, 1.f})};
	ray_dir.normalize();

    return {this->eye, ray_dir};
}

};
