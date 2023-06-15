#ifndef WHITTED_SHADER_HPP
#define WHITTED_SHADER_HPP

#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "utils/rgb.hpp"

class whitted_shader_t : public shader_t {

private:
    rgb_t<float> const background;
    unsigned const max_depth;

    rgb_t<float> direct_lighting(intersection_t const& isect) const noexcept;
    rgb_t<float> specular_reflection(
		intersection_t const& isect,
		size_t const depth
	) const noexcept;

    rgb_t<float> shade(ray_t const& ray, size_t const depth) const noexcept;

public:
    whitted_shader_t(
        std::unique_ptr<scene_t> scene,
        rgb_t<float> const& bg,
        unsigned const max_depth = 4
    ) noexcept;

    ~whitted_shader_t() noexcept;

    rgb_t<float> shade(ray_t const& ray) const noexcept override;
};

#endif //WHITTED_SHADER_HPP
