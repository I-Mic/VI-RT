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

namespace render {

class renderer_t {

protected:
    std::unique_ptr<cam::camera_t> cam;
    std::unique_ptr<shader::shader_t> shader;

public:
    renderer_t(
        std::unique_ptr<cam::camera_t> cam,
        std::unique_ptr<shader::shader_t> shader
    ) noexcept;

    virtual ~renderer_t() = default;

    virtual rgb::rgb_t<float> render_pixel(
        size_t const x,
        size_t const y
    ) const = 0;
};

};

#endif /* RENDERER_HPP */
