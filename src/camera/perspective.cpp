//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "camera/perspective.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"
#include <optional>

namespace cam {

perspective_t::perspective_t(size_t const w, size_t const h) : width{w}, height{h} {

    vec::vec3_t const f = this->at - this->eye;
    vec::vec3_t r = this->up.cross_product(f); r.normalize();

    this->c2w[0] = { r.x, r.y, r.z };
    this->c2w[1] = { this->up.x, this->up.y, this->up.z };
    this->c2w[2] = { f.x, f.y, f.z };
}

static vec::vec3_t matrix_product(std::array<std::array<float, 3>, 3> const& c2w, std::array<float, 3> const& p){

    std::array<float, 3> res { 0.f, 0.f, 0.f };

    for(size_t i = 0; i < 3; ++i)

        for(size_t j = 0; j < 3; ++j)

            res[i] += c2w[i][j] * p[j];

    return { res[0], res[1], res[2] };
}

std::optional<ray::ray_t> perspective_t::generate_ray(const int x, const int y, const float *cam_jitter){

    float const xs = 2 * (x + 0.5) / this->width - 1;
    float const ys = 2 * (y + 0.5) / this->height - 1;

    float const xc = xs * std::tan(this->field_of_view_w / 2);
    float const yc = ys * std::tan(this->field_of_view_h / 2);

    vec::vec3_t const ray_dir = matrix_product(this->c2w, {xc, yc, 1.f});

    return std::make_optional<ray::ray_t>(this->eye, ray_dir);
}

};
