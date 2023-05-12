#include "shader/path_tracer_shader.hpp"
#include "light/light.hpp"
#include "primitive/brdf/brdf.hpp"
#include "rays/ray.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <optional>

namespace shader {

path_tracer_shader_t::path_tracer_shader_t(
    std::unique_ptr<scene::scene_t> scene,
    rgb::rgb_t<float> const& bg,
    unsigned const max_depth,
    float const p_continue
) noexcept :
    shader_t{std::move(scene)},
    background{bg},
    max_depth{max_depth},
    p_continue{p_continue}
{
    std::srand(std::time(nullptr));
}

path_tracer_shader_t::~path_tracer_shader_t() noexcept {}


rgb::rgb_t<float> path_tracer_shader_t::direct_lighting(
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

rgb::rgb_t<float> path_tracer_shader_t::direct_lighting(
    ray::intersection_t const& isect
) const noexcept {

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    long const num_of_lights {lights_iter_end - lights_iter_begin};
    long const light_index {std::rand() % num_of_lights};
    return
        this->direct_lighting(isect, *(lights_iter_begin + light_index), brdf) *
        static_cast<float>(num_of_lights);
}

rgb::rgb_t<float> path_tracer_shader_t::specular_reflection(
    ray::intersection_t const& isect, unsigned const depth
) const noexcept {

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    float const cos {isect.gn.dot_product(isect.wo)};
    vec::vec3_t const rdir {2.f * cos * isect.gn - isect.wo};

    if(brdf->specular_exp() >= 1000.f){
        ray::ray_t specular_ray {isect.p, rdir};
        specular_ray.adjust_origin(isect.gn);

        return brdf->specular() * this->shade(specular_ray, depth + 1);
    }
    else {
        std::array<float, 2> const rand_pair {
            std::rand() / static_cast<float>(RAND_MAX),
            std::rand() / static_cast<float>(RAND_MAX)
        };

        float const cos_theta {std::pow(rand_pair[1], 1.f / (brdf->specular_exp() + 1))};
        float const aux_r1 {std::pow(rand_pair[1], 2.f / (brdf->specular_exp() + 1))};

        vec::vec3_t const s_around_n {
            static_cast<float>(std::cos(2.f * M_PI * rand_pair[0]) * std::sqrt(1.f - aux_r1)),
            static_cast<float>(std::sin(2.f * M_PI * rand_pair[0]) * std::sqrt(1.f - aux_r1)),
            cos_theta
        };

        float const pdf {
            static_cast<float>(
                (brdf->specular_exp() + 1.f) *
                std::pow(cos_theta, brdf->specular_exp()) /
                (2.f * M_PI)
            )
        };

        auto const& [rx, ry] {rdir.coordinate_system()};
        vec::vec3_t const sdir {s_around_n.rotate(rx, ry, rdir)};

        ray::ray_t specular_ray {isect.p, sdir};
        specular_ray.adjust_origin(isect.gn);

        return
            brdf->specular() *
            this->shade(specular_ray, depth + 1) *
            std::pow(cos_theta, brdf->specular_exp()) /
            (2.f * M_PI * pdf);
    }
}

rgb::rgb_t<float> path_tracer_shader_t::diffuse_reflection(
    ray::intersection_t const& isect,
    unsigned const depth
) const noexcept {

    auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
    std::unique_ptr<prim::brdf::brdf_t> const& brdf {
        *(brdfs_iter_begin + static_cast<long>(isect.material_index))
    };

    std::array<float, 2> const rand_pair {
        std::rand() / static_cast<float>(RAND_MAX),
        std::rand() / static_cast<float>(RAND_MAX)
    };

    vec::vec3_t const d_around_z {
        static_cast<float>(std::cos(2.f * M_PI * rand_pair[0]) * std::sqrt(1.f - rand_pair[1])),
        static_cast<float>(std::sin(2.f * M_PI * rand_pair[0]) * std::sqrt(1.f - rand_pair[1])),
        std::sqrt(rand_pair[1])
    };

    float const cos_theta {d_around_z.z};
    float const pdf {static_cast<float>(cos_theta / M_PI)};

    rgb::rgb_t<float> color {};

    auto const& [rx, ry] {isect.gn.coordinate_system()};
    vec::vec3_t const ddir {d_around_z.rotate(rx, ry, isect.gn)};

    ray::ray_t diffuse_ray {isect.p, ddir};
    diffuse_ray.adjust_origin(isect.gn);

    if(!isect.le.has_value())
        color += brdf->diffuse() * cos_theta * this->shade(diffuse_ray, depth + 1) / pdf;

    return color;
}

rgb::rgb_t<float> path_tracer_shader_t::shade(
    ray::ray_t const& ray, unsigned const depth
) const noexcept {

    rgb::rgb_t<float> color {};

    if(depth < this->max_depth){

        std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
        if(!isect.has_value())
            color = this->background;
        else if(isect.value().le.has_value())
            return isect.value().le.value();
        else if(depth < this->max_depth) {

            auto const& [brdfs_iter_begin, _] {this->scene->get_brdfs_iterator()};
            std::unique_ptr<prim::brdf::brdf_t> const& brdf {
                *(brdfs_iter_begin + static_cast<long>(isect.value().material_index))
            };

            std::array<float, 2> const rand_pair {
                std::rand() / static_cast<float>(RAND_MAX),
                std::rand() / static_cast<float>(RAND_MAX)
            };
            float const sp {brdf->specular().y() / (brdf->specular().y() + brdf->diffuse().y())};
            rgb::rgb_t<float> lcolor {};


            if(rand_pair[0] < sp){
                if(rand_pair[1] < this->p_continue){
                    lcolor += this->specular_reflection(isect.value(), depth) / sp;
                    lcolor /= this->p_continue;
                }
            }
            else
                if(rand_pair[1] < this->p_continue){
                    lcolor += this->diffuse_reflection(isect.value(), depth) / (1.f - sp);
                    lcolor /= this->p_continue;
                }


            color += this->direct_lighting(isect.value()) + lcolor;
        }
    }

    return color;
}

rgb::rgb_t<float> path_tracer_shader_t::shade(ray::ray_t const& ray) const noexcept {
    return this->shade(ray, 0);
}

};
