//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <optional>

#include "rays/ray.hpp"

// based on pbrt book, sec 6.1, pag. 356
namespace cam {

class camera_t {

public:

    virtual ~camera_t();

    virtual std::optional<ray::ray_t> generate_ray(const int x, const int y, const float* cam_jitter = nullptr) = 0;
};

};

#endif /* CAMERA_HPP */
