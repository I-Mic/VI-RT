#include <memory>
#include <cassert>

#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/brdf.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "shader/ambient_shader.hpp"
#include "scene/scene.hpp"
#include "utils/rgb.hpp"


namespace shader {

ambient_shader_t::ambient_shader_t(
    std::unique_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg
) noexcept : shader_t{std::move(scene)}, background{bg} {}

ambient_shader_t::~ambient_shader_t() noexcept {}

rgb::rgb_t<float> ambient_shader_t::shade(
    std::optional<ray::ray_t> const& ray
) const noexcept
{

    std::optional<ray::intersection_t> const isect {this->scene->trace(ray.value())};
    if(!isect.has_value())
        return this->background;


    rgb::rgb_t<float> color {};

    prim::brdf::phong_t const* const phong {
        dynamic_cast<prim::brdf::phong_t*>(isect.value().f.get())
    };
    assert(phong != nullptr);

    this->scene->compute_ambient_color(phong->ka, color);
    return color;
}

};
