#include "renderer/standard_renderer.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/rgb.hpp"

namespace render {

standard_renderer_t::standard_renderer_t(
    std::unique_ptr<cam::camera_t> cam,
    std::unique_ptr<shader::shader_t> shader
) noexcept : renderer_t{std::move(cam), std::move(shader)} {}

standard_renderer_t::~standard_renderer_t() noexcept {}

rgb::rgb_t<float> standard_renderer_t::render_pixel(
    size_t const x, size_t const y
) const {

    std::optional<ray::ray_t> const primary_ray {this->cam->generate_ray(x, y)};

    rgb::rgb_t<float> const color {this->shader->shade(primary_ray)};

    return color;
}

};
