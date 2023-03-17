#include "renderer/renderer.hpp"


namespace render {

renderer_t::renderer_t(
    std::unique_ptr<cam::camera_t> cam, 
    std::unique_ptr<shader::shader_t> shader
) noexcept :
    cam{std::move(cam)},
    shader{std::move(shader)} {}

};
