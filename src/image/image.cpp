#include "image/image.hpp"

namespace img {

image_t::image_t(
    std::unique_ptr<render::renderer_t> renderer,
    size_t const w, size_t const h
) noexcept :
    image_plane{std::make_unique<rgb::rgb_t<float>[]>(w * h)},
    renderer{std::move(renderer)}, width{w}, height{h} {}

image_t::~image_t(){}

};