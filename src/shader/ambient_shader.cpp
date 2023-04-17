#include <memory>

#include "light/light.hpp"
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
) noexcept :
    shader_t{std::move(scene)},
    background{bg} {}

ambient_shader_t::~ambient_shader_t() noexcept {}

rgb::rgb_t<float> ambient_shader_t::shade(ray::ray_t const& ray) const noexcept {

    std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
    if(!isect.has_value())
        return this->background;


    rgb::rgb_t<float> color {};

    std::pair<scene::lights_iter_t, scene::lights_iter_t> const lights_range {
        this->scene->get_lights_iterator()
    };

    std::pair<scene::brdfs_iter_t, scene::brdfs_iter_t> const brdfs_range {
        this->scene->get_brdfs_iterator()
    };

    for(scene::lights_iter_t li {lights_range.first}; li < lights_range.second; ++li){

        std::unique_ptr<light::light_t> const& l {*li};

        if(l->type == light::light_type_t::AMBIENT_LIGHT){

            std::unique_ptr<prim::brdf::brdf_t> const& brdf {
                *(brdfs_range.first + static_cast<long>(isect.value().material_index))
            };

            color += brdf->ambient(l->compute_radiance({}));
        }
    }

    return color;
}

};
