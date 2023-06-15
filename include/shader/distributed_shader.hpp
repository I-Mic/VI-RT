#ifndef DISTRIBUTED_SHADER_HPP
#define DISTRIBUTED_SHADER_HPP

#include "shader/shader.hpp"

class distributed_shader_t : public shader_t {

private:
    rgb_t<float> const background;
    unsigned const max_depth;
    bool const use_monte_carlo_sampling;

    rgb_t<float> direct_lighting(
        intersection_t const& isect,
        std::unique_ptr<light_t> const& light,
        std::unique_ptr<brdf_t> const& brdf
    ) const noexcept;
    rgb_t<float> direct_lighting(intersection_t const& isect) const noexcept;
    rgb_t<float> specular_reflection(
		intersection_t const& isect,
		unsigned const depth
	) const noexcept;

    rgb_t<float> shade(ray_t const& ray, unsigned const depth) const noexcept;

public:
    distributed_shader_t(
        std::unique_ptr<scene_t> scene,
        rgb_t<float> const& bg,
        unsigned const max_depth = 4,
        bool const use_monte_carlo_sampling = true
    ) noexcept;

    ~distributed_shader_t() noexcept;

    rgb_t<float> shade(ray_t const& ray) const noexcept override;
};

#endif //DISTRIBUTED_SHADER_HPP
