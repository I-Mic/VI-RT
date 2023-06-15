#include "renderer/renderer.hpp"

renderer_t::renderer_t(
    std::unique_ptr<camera_t> cam,
    std::unique_ptr<shader_t> shader,
    unsigned const samples_per_pixel
) noexcept :
    cam{std::move(cam)},
    shader{std::move(shader)},
    samples_per_pixel{samples_per_pixel} {}
