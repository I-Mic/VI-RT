#ifndef STANDARD_RENDERER_HPP
#define STANDARD_RENDERER_HPP

#include "renderer/renderer.hpp"

class standard_renderer_t : public renderer_t {

public:

    standard_renderer_t(
        std::unique_ptr<camera_t> cam,
        std::unique_ptr<shader_t> shader,
        unsigned const samples_per_pixel = 32
    ) noexcept;

    ~standard_renderer_t() noexcept = default;

    rgb_t<float> render_pixel(size_t const x, size_t const y) const override;
};

#endif //STANDARD_RENDERER_HPP
