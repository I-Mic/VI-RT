#ifndef STANDARD_RENDERER_HPP
#define STANDARD_RENDERER_HPP

#include "renderer/renderer.hpp"


namespace render {

class standard_renderer_t : public renderer_t {

public:

    standard_renderer_t(
        std::shared_ptr<cam::camera_t> cam,
        std::shared_ptr<scene::scene_t> scene,
        std::shared_ptr<img::image_t> img,
        std::shared_ptr<shader::shader_t> shader
    ) noexcept;

    ~standard_renderer_t() noexcept;

    void render() const override;
};

};

#endif
