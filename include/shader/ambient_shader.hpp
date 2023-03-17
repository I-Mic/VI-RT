#ifndef AMBIENT_SHADER_HPP
#define AMBIENT_SHADER_HPP

#include <optional>

#include "rays/ray.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "utils/rgb.hpp"

namespace shader {

class ambient_shader_t : public shader_t {

private:
	rgb::rgb_t<float> background;

public:
	ambient_shader_t(std::unique_ptr<scene::scene_t> scene, rgb::rgb_t<float> const& bg) noexcept;

	~ambient_shader_t() noexcept;

	rgb::rgb_t<float> shade(std::optional<ray::ray_t> const& ray) const noexcept override;
};

};

#endif //AMBIENT_SHADER_HPP
