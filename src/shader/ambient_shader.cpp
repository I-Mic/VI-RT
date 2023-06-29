#include "shader/ambient_shader.hpp"


ambient_shader_t::ambient_shader_t(
    std::unique_ptr<scene_t> scene,
    rgb_t<float> const& bg
) noexcept :
    shader_t{std::move(scene)},
    background{bg} {}

rgb_t<float> ambient_shader_t::shade(ray_t const& ray) const noexcept {

    std::optional<intersection_t> const isect {this->scene->trace(ray)};
    if(!isect.has_value())
        return this->background;

    rgb_t<float> color {};

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    material_t const* const mat {
        isect.value().material_index.has_value()
            ? this->scene->material_at(isect.value().material_index.value())
            : &material_t::DEFAULT
    };

    for(scene_t::lights_citer_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light_t> const& light {*li};

        if(light->type == light_type_t::AMBIENT_LIGHT)
            color += mat->ka * light->get_properties().radiance.value();
    }

    return color;
}
