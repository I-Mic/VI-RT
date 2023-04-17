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
    rgb::rgb_t<float> const& bg
) noexcept :
    shader_t{std::move(scene)},
    background{bg} {}

whitted_shader_t::~whitted_shader_t() noexcept {}

rgb::rgb_t<float> whitted_shader_t::direct_lighting(
	ray::intersection_t const& isect
) const noexcept {

    std::pair<scene::lights_iter_t, scene::lights_iter_t> const lights_range {
        this->scene->get_lights_iterator()
    };

    std::pair<scene::brdfs_iter_t, scene::brdfs_iter_t> const brdfs_range {
        this->scene->get_brdfs_iterator()
    };

	std::unique_ptr<prim::brdf::brdf_t> const& brdf {
		*(brdfs_range.first + static_cast<long>(isect.material_index))
	};

	rgb::rgb_t<float> color {};

    for(scene::lights_iter_t li {lights_range.first}; li < lights_range.second; ++li){

        std::unique_ptr<light::light_t> const& l {*li};

		switch(l->type){

		case light::light_type_t::AMBIENT_LIGHT:
			color += brdf->ambient(l->compute_radiance({}));
			break;

		case light::light_type_t::POINT_LIGHT: {

			//TODO
   			vec::vec3_t point {};
			l->sample_l(0.f, 0.f);

			break;
		}

		case light::light_type_t::NO_LIGHT:
			break;
		}
	}

    return color;
}

rgb::rgb_t<float> whitted_shader_t::specular_reflection(
	ray::intersection_t const& isect
) const noexcept {

	//TODO
	return {};
}

rgb::rgb_t<float> whitted_shader_t::shade(ray::ray_t const& ray) const noexcept {

    std::optional<ray::intersection_t> const isect {this->scene->trace(ray)};
    if(!isect.has_value())
        return this->background;

    return this->specular_reflection(isect.value()) + this->direct_lighting(isect.value());
}

};
