#ifndef PATH_TRACER_SHADER_HPP
#define PATH_TRACER_SHADER_HPP

#include "shader/shader.hpp"

class path_tracer_shader_t : public shader_t {

private:
    rgb_t<float> const background;
    unsigned const max_depth;
    float const p_continue;

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

    rgb_t<float> diffuse_reflection(
        intersection_t const& isect,
        unsigned const depth
    ) const noexcept;

    rgb_t<float> shade(ray_t const& ray, unsigned const depth) const noexcept;

public:
    path_tracer_shader_t(
        std::unique_ptr<scene_t> scene,
        rgb_t<float> const& bg,
        unsigned const max_depth = 4,
        float const p_continue = 0.5f
    ) noexcept;

    ~path_tracer_shader_t() noexcept;

    rgb_t<float> shade(ray_t const& ray) const noexcept override;
};

#endif //PATH_TRACER_SHADER_HPP
