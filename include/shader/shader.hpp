#ifndef SHADER_HPP
#define SHADER_HPP

#include <memory>

#include "scene/scene.hpp"
#include "utils/rgb.hpp"

class shader_t {

protected:
	std::unique_ptr<scene_t> const scene;

public:
	shader_t(std::unique_ptr<scene_t> scene) noexcept : scene{std::move(scene)} {}

	virtual ~shader_t() = default;

	virtual rgb_t<float> shade(ray_t const& ray) const noexcept = 0;
};

#endif //SHADER_HPP
