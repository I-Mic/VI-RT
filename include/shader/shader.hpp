#ifndef SHADER_HPP
#define SHADER_HPP

#include <memory>

#include "rays/intersection.hpp"
#include "scene/scene.hpp"
#include "utils/rgb.hpp"

namespace shader {

class shader_t {

protected:
	std::shared_ptr<scene::scene_t> scene;

public:
	shader_t(std::shared_ptr<scene::scene_t> scene) noexcept : scene{scene} {}

	virtual ~shader_t() = default;

	virtual rgb::rgb_t<float> shade(
		bool const intersected,
		ray::intersection_t const& isect
	) const noexcept = 0;
};

};

#endif //SHADER_HPP
