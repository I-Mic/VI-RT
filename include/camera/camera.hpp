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

    size_t const width;
    size_t const height;

    camera_t(size_t const width, size_t const height) noexcept;

    virtual ~camera_t();

    virtual std::optional<ray::ray_t> generate_ray(
        int const x, int const y,
        float const* const cam_jitter = nullptr
    ) const = 0;
};

};

#endif /* CAMERA_HPP */
