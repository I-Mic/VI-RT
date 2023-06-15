//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef RAY_HPP
#define RAY_HPP

#include "utils/vector.hpp"

struct ray_t {

public:

    static float constexpr EPSILON {1e-3};

    vec3_t org; // ray origin
    vec3_t dir; // ray direction

    ray_t(){}
    ray_t(vec3_t const& o, vec3_t const& d): org{o}, dir{d} {}

    void adjust_origin(vec3_t const& normal) noexcept {

        vec3_t offset {EPSILON * normal};
        if(this->dir.dot_product(normal) < 0.f)
            offset *= -1.f;

        this->org.x += offset.x;
        this->org.y += offset.y;
        this->org.z += offset.z;
    }
};

#endif /* RAY_HPP */
