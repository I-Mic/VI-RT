//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "camera/camera.hpp"
#include "shader/shader.hpp"

class renderer_t {

protected:
    std::unique_ptr<camera_t> const cam;
    std::unique_ptr<shader_t> const shader;
    unsigned const samples_per_pixel;

public:
    renderer_t(
        std::unique_ptr<camera_t> cam,
        std::unique_ptr<shader_t> shader,
        unsigned const samples_per_pixel
    ) noexcept :
        cam{std::move(cam)},
        shader{std::move(shader)},
        samples_per_pixel{samples_per_pixel} {}

    virtual ~renderer_t() noexcept = default;

    virtual rgb_t<float> render_pixel(
        size_t const x,
        size_t const y
    ) const = 0;
};

#endif /* RENDERER_HPP */
