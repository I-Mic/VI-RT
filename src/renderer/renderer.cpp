#include "renderer/renderer.hpp"


namespace render {

renderer_t::renderer_t(
    std::shared_ptr<cam::camera_t> cam, 
    std::shared_ptr<scene::scene_t> scene,
    std::shared_ptr<img::image_t> img,
    std::shared_ptr<shader::shader_t> shader
) noexcept :
    cam{std::move(cam)},
    scene{std::move(scene)},
    img{std::move(img)},
    shader{std::move(shader)} {}

};
