#include <memory>

#include "light/light.hpp"
#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/brdf.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "shader/whitted_shader.hpp"
#include "scene/scene.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"


namespace shader {

whitted_shader_t::whitted_shader_t(
    std::unique_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg,
    unsigned const max_depth
) noexcept :
    shader_t{std::move(scene)},
    background{bg},
    max_depth{max_depth} {}

whitted_shader_t::~whitted_shader_t() noexcept {}

rgb::rgb_t<float> whitted_shader_t::direct_lighting(
    ray::intersection_t const& isect
) const noexcept {

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    rgb::rgb_t<float> color {};

    for(scene::lights_iter_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light::light_t> const& light {*li};

        switch(light->type){

        case light::light_type_t::AMBIENT_LIGHT:
            color += brdf->ambient() * light->radiance({});
            break;

        case light::light_type_t::POINT_LIGHT: {

            auto const& [pos, radiance,_] {light->sample_l(0.f, 0.f)};

            vec::vec3_t ldir {pos - isect.p};
            float const ldistance {ldir.norm()};
            ldir.normalize();

            float const cosl {ldir.dot_product(isect.sn)};
            if(cosl > 0.f){

                ray::ray_t shadow {isect.p, ldir};
                shadow.adjust_origin(isect.gn);

                if(this->scene->is_visible(shadow, ldistance - ray::ray_t::EPSILON))
                    color += brdf->diffuse() * radiance * cosl;
            }

            break;
        }

        default:
            break;
        }
    }


    return color;
}

rgb::rgb_t<float> whitted_shader_t::specular_reflection(
    ray::intersection_t const& isect
) const noexcept {

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    if(brdf->specular().is_zero())
        return {};

    float const cos {isect.gn.dot_product(isect.wo)};
    vec::vec3_t const rdir {2.f * cos * isect.gn - isect.wo};
    ray::ray_t specular {isect.p, rdir};
    specular.adjust_origin(isect.gn);

    return this->shade(specular);
}

rgb::rgb_t<float> whitted_shader_t::shade(ray::ray_t const& ray) const noexcept {

    static unsigned recursion_depth {0};
    ++recursion_depth;

    rgb::rgb_t<float> color {};

    if(recursion_depth <= this->max_depth){

        std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
        if(!isect.has_value())
            color = this->background;
        else {
            rgb::rgb_t<float> const specular {this->specular_reflection(isect.value())};
            rgb::rgb_t<float> const direct {this->direct_lighting(isect.value())};
            color = specular + direct;
        }
    }

    --recursion_depth;

    return color;
}

};
