#include "shader/distributed_shader.hpp"
#include "light/light.hpp"
#include "primitive/brdf/brdf.hpp"
#include "utils/vector.hpp"

#include <ctime>
#include <cstdlib>
#include <optional>

namespace shader {

distributed_shader_t::distributed_shader_t(
    std::unique_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg,
    unsigned const max_depth,
    bool const use_monte_carlo_sampling
) noexcept :
    shader_t{std::move(scene)},
    background{bg},
    max_depth{max_depth},
    use_monte_carlo_sampling{use_monte_carlo_sampling}
{
    std::srand(std::time(nullptr));
}

distributed_shader_t::~distributed_shader_t() noexcept {}


rgb::rgb_t<float> distributed_shader_t::direct_lighting(
    ray::intersection_t const& isect,
    std::unique_ptr<light::light_t> const& light,
    std::unique_ptr<prim::brdf::brdf_t> const& brdf
) const noexcept {

    rgb::rgb_t<float> color {};

    switch(light->type){

    case light::light_type_t::AMBIENT_LIGHT:
        color += brdf->ambient() * light->get_properties().radiance.value();
        break;

    case light::light_type_t::POINT_LIGHT: {

        light::light_properties_t const lprops {light->get_properties()};

        vec::vec3_t ldir {lprops.point.value() - isect.p};
        float const ldistance {ldir.norm()};
        ldir.normalize();

        float const cosl {ldir.dot_product(isect.sn)};
        if(cosl > 0.f){

            ray::ray_t shadow {isect.p, ldir};
            shadow.adjust_origin(isect.gn);

            if(this->scene->is_visible(shadow, ldistance - ray::ray_t::EPSILON))
                color += brdf->diffuse() * lprops.radiance.value() * cosl;
        }

        break;
    }

    case light::light_type_t::AREA_LIGHT: {

        std::array<float, 2> const rand_pair {
            std::rand() / static_cast<float>(RAND_MAX),
            std::rand() / static_cast<float>(RAND_MAX)
        };
        light::light_properties_t const lprops {
            light->get_properties(
                {
                    .rand_pair{std::make_optional(rand_pair)},
                }
            )
        };

        vec::vec3_t ldir {lprops.point.value() - isect.p};
        float const ldistance {ldir.norm()};
        ldir.normalize();

        float const cosl {ldir.dot_product(isect.sn)};
        float const cosl_la {ldir.dot_product(lprops.light_geom->normal)};
        if(cosl > 0.f && cosl_la <= 0.f){

            ray::ray_t shadow {isect.p, ldir};
            shadow.adjust_origin(isect.gn);

            if(this->scene->is_visible(shadow, ldistance - ray::ray_t::EPSILON))
                color += brdf->diffuse() * lprops.radiance.value() * cosl / lprops.pdf.value();
        }
    }

    default:
        break;
    }

    return color;
}

rgb::rgb_t<float> distributed_shader_t::direct_lighting(
    ray::intersection_t const& isect
) const noexcept {

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    rgb::rgb_t<float> color {};

    if(!this->use_monte_carlo_sampling)
        for(scene::lights_iter_t li {lights_iter_begin}; li != lights_iter_end; ++li)
            color += this->direct_lighting(isect, *li, brdf);
    else {
        long const num_of_lights {lights_iter_end - lights_iter_begin};
        long const light_index {std::rand() % num_of_lights};
        color +=
            this->direct_lighting(isect, *(lights_iter_begin + light_index), brdf) *
            static_cast<float>(num_of_lights);
    }

    return color;
}

rgb::rgb_t<float> distributed_shader_t::specular_reflection(
    ray::intersection_t const& isect, unsigned const depth
) const noexcept {

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    float const cos {isect.gn.dot_product(isect.wo)};
    vec::vec3_t const rdir {2.f * cos * isect.gn - isect.wo};
    ray::ray_t specular {isect.p, rdir};
    specular.adjust_origin(isect.gn);

    return brdf->specular() * this->shade(specular, depth + 1);
}


rgb::rgb_t<float> distributed_shader_t::shade(
    ray::ray_t const& ray, unsigned const depth
) const noexcept {

    rgb::rgb_t<float> color {};

    if(depth < this->max_depth){

        std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
        if(!isect.has_value())
            color = this->background;
        else if(isect.value().le.has_value())
            return isect.value().le.value();
        else {
            rgb::rgb_t<float> const specular {this->specular_reflection(isect.value(), depth)};
            rgb::rgb_t<float> const direct {this->direct_lighting(isect.value())};
            color = specular + direct;
        }
    }

    return color;
}

rgb::rgb_t<float> distributed_shader_t::shade(ray::ray_t const& ray) const noexcept {
    return this->shade(ray, 0);
}

};