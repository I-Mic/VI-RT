#include "shader/whitted_shader.hpp"
#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/lambert.hpp"


whitted_shader_t::whitted_shader_t(
    std::unique_ptr<scene_t> scene,
    std::unique_ptr<diffuse_brdf_t> diffuse_brdf,
    std::unique_ptr<specular_brdf_t> specular_brdf,
    rgb_t<float> const& bg,
    unsigned const max_depth
) noexcept :
    shader_t{std::move(scene)},
    background{bg},
    diffuse_brdf{std::move(diffuse_brdf)},
    specular_brdf{std::move(specular_brdf)},
    max_depth{max_depth} {}

rgb_t<float> whitted_shader_t::direct_lighting(
    intersection_t const& isect
) const noexcept {

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    material_t const* const mat {this->scene->material_at(isect.material_index)};

    rgb_t<float> color {};

    for(scene_t::lights_citer_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light_t> const& light {*li};

        switch(light->type){

        case light_type_t::AMBIENT_LIGHT:
            color += mat->ka * light->get_properties().radiance.value();
            break;

        case light_type_t::POINT_LIGHT: {

            light_properties_t const lprops {light->get_properties()};

            vec3_t wi {lprops.point.value() - isect.p};
            float const ldistance {wi.norm()};
            wi.normalize();

            ray_t shadow {isect.p, wi};
            shadow.adjust_origin(isect.sn);
            if(this->scene->is_visible(shadow, ldistance - ray_t::EPSILON)){
                brdf_data_t const data {
                    .wi{std::make_optional(wi)},
                    .sn{std::make_optional(isect.sn)},
                    .material{mat}
                };
                color += this->diffuse_brdf->eval_diffuse(data) * lprops.radiance.value();
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

    material_t const* const mat {this->scene->material_at(isect.material_index)};

    std::array<float, 2> const rand_pair {0.5f, 0.5f};

    brdf_data_t data {
        .wo{std::make_optional(isect.wo)},
        .sn{std::make_optional(isect.sn)},
        .rand_pair{std::make_optional(rand_pair)},
        .material{mat}
    };

    auto const& [wi, pdf] {this->specular_brdf->sample_specular(data)};
    data.wi = std::make_optional(wi);

    ray_t spec_ray {isect.p, wi};
    spec_ray.adjust_origin(isect.sn);

    if(!isect.le.has_value())
        return this->specular_brdf->eval_specular(data) * this->shade(spec_ray, depth + 1) / pdf;

    return {};
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
