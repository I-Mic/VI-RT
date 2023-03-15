//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "camera/camera.hpp"
#include "image/image.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"

namespace render {

class renderer_t {
protected:
    std::shared_ptr<cam::camera_t> cam;
    std::shared_ptr<scene::scene_t> scene;
    std::shared_ptr<img::image_t> img;
    std::shared_ptr<shader::shader_t> shader;
public:
    renderer_t(
        std::shared_ptr<cam::camera_t> cam,
        std::shared_ptr<scene::scene_t> scene,
        std::shared_ptr<img::image_t> img,
        std::shared_ptr<shader::shader_t> shader
    ) noexcept;

    virtual ~renderer_t() = default;

    virtual void render() const = 0;
};

};

#endif /* RENDERER_HPP */
