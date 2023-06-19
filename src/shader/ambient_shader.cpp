#include "shader/ambient_shader.hpp"


ambient_shader_t::ambient_shader_t(
    std::unique_ptr<scene_t> scene,
    rgb_t<float> const& bg
) noexcept :
    shader_t{std::move(scene)},
    background{bg} {}

ambient_shader_t::~ambient_shader_t() noexcept {}

rgb_t<float> ambient_shader_t::shade(ray_t const& ray) const noexcept {

    std::optional<intersection_t> const isect {this->scene->trace(ray)};
    if(!isect.has_value())
        return this->background;

    rgb_t<float> color {};

    auto const& [lights_iter_begin, lights_iter_end] {this->scene->get_lights_iterator()};

    auto const& [materials_iter_begin, _] {this->scene->get_materials_iterator()};
    material_t const& mat {
        *(materials_iter_begin + static_cast<long>(isect.value().material_index))
    };

    for(lights_iter_t li {lights_iter_begin}; li != lights_iter_end; ++li){

        std::unique_ptr<light_t> const& light {*li};

        if(light->type == light_type_t::AMBIENT_LIGHT)
            color += mat.ka * light->get_properties().radiance.value();
    }

    return color;
}
