#ifndef SHADER_HPP
#define SHADER_HPP

#include <memory>

#include "scene/scene.hpp"
#include "utils/rgb.hpp"

namespace shader {

class shader_t {

protected:
	std::unique_ptr<scene::scene_t> scene;

public:
	shader_t(std::unique_ptr<scene::scene_t> scene) noexcept : scene{std::move(scene)} {}

	virtual ~shader_t() = default;

	virtual rgb::rgb_t<float> shade(ray::ray_t const& ray) const noexcept = 0;
};

};

#endif //SHADER_HPP
