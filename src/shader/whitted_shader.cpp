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


whitted_shader_t::whitted_shader_t(
    std::unique_ptr<scene_t> scene,
    rgb_t<float> const& bg,
    unsigned const max_depth
) noexcept :
    shader_t{std::move(scene)},
    background{bg},
    max_depth{max_depth} {}

whitted_shader_t::~whitted_shader_t() noexcept {}

rgb_t<float> whitted_shader_t::direct_lighting(
    intersection_t const& isect
) const noexcept {

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    rgb_t<float> color {};

    for(lights_iter_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light_t> const& light {*li};

        switch(light->type){

        case light_type_t::AMBIENT_LIGHT:
            color += brdf->ambient() * light->get_properties().radiance.value();
            break;

        case light_type_t::POINT_LIGHT: {

            light_properties_t const lprops {light->get_properties()};

            vec3_t ldir {lprops.point.value() - isect.p};
            float const ldistance {ldir.norm()};
            ldir.normalize();

            float const cosl {ldir.dot_product(isect.sn)};
            if(cosl > 0.f){

                ray_t shadow {isect.p, ldir};
                shadow.adjust_origin(isect.gn);

                if(this->scene->is_visible(shadow, ldistance - ray_t::EPSILON))
                    color += brdf->diffuse() * lprops.radiance.value() * cosl;
            }

            break;
        }

        default:
            break;
        }
    }


    return color;
}

rgb_t<float> whitted_shader_t::specular_reflection(
    intersection_t const& isect, size_t const depth
) const noexcept {

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    float const cos {isect.gn.dot_product(isect.wo)};
    vec3_t const rdir {2.f * cos * isect.gn - isect.wo};
    ray_t specular {isect.p, rdir};
    specular.adjust_origin(isect.gn);

    return brdf->specular() * this->shade(specular, depth + 1);
}

rgb_t<float> whitted_shader_t::shade(ray_t const& ray, size_t const depth) const noexcept {

    rgb_t<float> color {};

    if(depth < this->max_depth){

        std::optional<intersection_t> const isect {this->scene->trace(ray)};
        if(!isect.has_value())
            color = this->background;
        else {
            rgb_t<float> const specular {this->specular_reflection(isect.value(), depth)};
            rgb_t<float> const direct {this->direct_lighting(isect.value())};
            color = specular + direct;
        }
    }

    return color;
}

rgb_t<float> whitted_shader_t::shade(ray_t const& ray) const noexcept {
    return this->shade(ray, 0);
}
