#ifndef DISTRIBUTED_SHADER_HPP
#define DISTRIBUTED_SHADER_HPP

#include "shader/shader.hpp"
#include "primitive/brdf/brdf.hpp"

class distributed_shader_t : public shader_t {

private:
    rgb_t<float> const background;
    unsigned const max_depth;
    std::unique_ptr<diffuse_brdf_t> const diffuse_brdf;
    std::unique_ptr<specular_brdf_t> const specular_brdf;

    rgb_t<float> direct_lighting(
        intersection_t const& isect,
        std::unique_ptr<light_t> const& light,
        material_t const& mat
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
        unsigned const max_depth = 4
    ) noexcept;

    ~distributed_shader_t() noexcept;

    rgb_t<float> shade(ray_t const& ray) const noexcept override;
};

#endif //DISTRIBUTED_SHADER_HPP
