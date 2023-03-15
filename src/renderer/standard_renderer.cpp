#include "renderer/standard_renderer.hpp"

namespace render {

standard_renderer_t::standard_renderer_t(
    std::shared_ptr<cam::camera_t> cam,
    std::shared_ptr<scene::scene_t> scene,
    std::shared_ptr<img::image_t> img,
    std::shared_ptr<shader::shader_t> shader
) noexcept : renderer_t{cam, scene, img, shader} {}

standard_renderer_t::~standard_renderer_t() noexcept {}

void standard_renderer_t::render() const {
    
}

};
