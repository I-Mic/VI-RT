#ifndef AMBIENT_SHADER_HPP
#define AMBIENT_SHADER_HPP

#include "rays/ray.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "utils/rgb.hpp"

class ambient_shader_t : public shader_t {

private:
	rgb_t<float> const background;

public:
	ambient_shader_t(std::unique_ptr<scene_t> scene, rgb_t<float> const& bg) noexcept;

	~ambient_shader_t() noexcept = default;

	rgb_t<float> shade(ray_t const& ray) const noexcept override;
};

#endif //AMBIENT_SHADER_HPP
