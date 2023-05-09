#ifndef PATH_TRACER_SHADER_HPP
#define PATH_TRACER_SHADER_HPP

#include "shader/shader.hpp"

namespace shader {

class path_tracer_shader_t : public shader_t {

private:
    rgb::rgb_t<float> const background;
    unsigned const max_depth;
    bool const use_monte_carlo_sampling;

    rgb::rgb_t<float> direct_lighting(
        ray::intersection_t const& isect,
        std::unique_ptr<light::light_t> const& light,
        std::unique_ptr<prim::brdf::brdf_t> const& brdf
    ) const noexcept;
    rgb::rgb_t<float> direct_lighting(ray::intersection_t const& isect) const noexcept;
    rgb::rgb_t<float> specular_reflection(
		ray::intersection_t const& isect,
		size_t const depth
	) const noexcept;

    rgb::rgb_t<float> shade(ray::ray_t const& ray, size_t const depth) const noexcept;

public:
    path_tracer_shader_t(
        std::unique_ptr<scene::scene_t> scene,
        rgb::rgb_t<float> const& bg,
        unsigned const max_depth = 4,
        bool const use_monte_carlo_sampling = true
    ) noexcept;

    ~path_tracer_shader_t() noexcept;

    rgb::rgb_t<float> shade(ray::ray_t const& ray) const noexcept override;
};

}

#endif //PATH_TRACER_SHADER_HPP
