#include <memory>

#include "light/light.hpp"
#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/brdf.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "shader/ambient_shader.hpp"
#include "scene/scene.hpp"
#include "utils/rgb.hpp"

#include <iostream>


namespace shader {

ambient_shader_t::ambient_shader_t(
    std::unique_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg
) noexcept :
    shader_t{std::move(scene)},
    background{bg} {}

ambient_shader_t::~ambient_shader_t() noexcept {}

rgb::rgb_t<float> ambient_shader_t::shade(ray::ray_t const& ray) const noexcept {

    std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
    if(!isect.has_value())
        return this->background;

    rgb::rgb_t<float> color {};

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.value().material_index))
    };

    for(scene::lights_iter_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light::light_t> const& light {*li};

        if(light->type == light::light_type_t::AMBIENT_LIGHT)
            color += brdf->ambient() * light->get_properties().radiance.value();
    }

    return color;
}

};
