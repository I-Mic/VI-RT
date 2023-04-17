#ifndef WHITTED_SHADER_HPP
#define WHITTED_SHADER_HPP

#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "utils/rgb.hpp"

namespace shader {

class whitted_shader_t : public shader_t {

private:
	rgb::rgb_t<float> const background;

	rgb::rgb_t<float> direct_lighting(ray::intersection_t const& isect) const noexcept;
	rgb::rgb_t<float> specular_reflection(ray::intersection_t const& isect) const noexcept;

public:
	whitted_shader_t(std::unique_ptr<scene::scene_t> scene, rgb::rgb_t<float> const& bg) noexcept;

	~whitted_shader_t() noexcept;

	rgb::rgb_t<float> shade(ray::ray_t const& ray) const noexcept override;
};

};

#endif //WHITTED_SHADER_HPP
