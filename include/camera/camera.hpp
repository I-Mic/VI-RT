//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rays/ray.hpp"

// based on pbrt book, sec 6.1, pag. 356
namespace cam {

class camera_t {

protected:
    size_t const width;
    size_t const height;

public:
    camera_t(size_t const width, size_t const height) noexcept;

    virtual ~camera_t();

    virtual ray::ray_t generate_ray(
        size_t const x, size_t const y,
        std::array<float, 2> = {0.5, 0.5} 
    ) const = 0;
};

};

#endif /* CAMERA_HPP */
