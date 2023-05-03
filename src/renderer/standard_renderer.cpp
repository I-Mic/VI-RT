#include "renderer/standard_renderer.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/rgb.hpp"

#include <cstdlib>
#include <ctime>

namespace render {

standard_renderer_t::standard_renderer_t(
    std::unique_ptr<cam::camera_t> cam,
    std::unique_ptr<shader::shader_t> shader,
    unsigned const samples_per_pixel
) noexcept :
    renderer_t{std::move(cam), std::move(shader), samples_per_pixel}
{
    std::srand(std::time(nullptr));
}

standard_renderer_t::~standard_renderer_t() noexcept {}

rgb::rgb_t<float> standard_renderer_t::render_pixel(
    size_t const x, size_t const y
) const {

    rgb::rgb_t<float> color {};

    for(unsigned spp {0}; spp < this->samples_per_pixel; ++spp){

        std::array<float, 2> const jitter {
            std::rand() / static_cast<float>(RAND_MAX),
            std::rand() / static_cast<float>(RAND_MAX)
        };
        ray::ray_t const primary_ray {this->cam->generate_ray(x, y, jitter)};

        color += this->shader->shade(primary_ray);
    }

    color /= static_cast<float>(this->samples_per_pixel);

    return color;
}

};
