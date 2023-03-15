#include "shader/ambient_shader.hpp"
#include "scene/scene.hpp"
#include "utils/rgb.hpp"
#include <memory>


namespace shader {

ambient_shader_t::ambient_shader_t(
    std::shared_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg
) noexcept : shader_t{std::move(scene)}, background{bg} {}

ambient_shader_t::~ambient_shader_t() noexcept {}

rgb::rgb_t<float> ambient_shader_t::shade(
    bool const intersected, ray::intersection_t const& isect
) const noexcept 
{
    return {};
}

};
