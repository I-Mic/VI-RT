#ifndef DISTRIBUTED_SHADER_HPP
#define DISTRIBUTED_SHADER_HPP

#include "shader/shader.hpp"

namespace shader {

class distributed_shader_t : public shader_t {

private:
    rgb::rgb_t<float> const background;
    unsigned const max_depth;

    rgb::rgb_t<float> direct_lighting(ray::intersection_t const& isect) const noexcept;
    rgb::rgb_t<float> specular_reflection(
		ray::intersection_t const& isect,
		size_t const depth
	) const noexcept;

    rgb::rgb_t<float> shade(ray::ray_t const& ray, size_t const depth) const noexcept;

public:
    distributed_shader_t(
        std::unique_ptr<scene::scene_t> scene,
        rgb::rgb_t<float> const& bg,
        unsigned const max_depth = 4
    ) noexcept;

    ~distributed_shader_t() noexcept;

    rgb::rgb_t<float> shade(ray::ray_t const& ray) const noexcept override;
};

}

#endif //DISTRIBUTED_SHADER_HPP
