#include "image/image.hpp"

image_t::image_t(
    std::unique_ptr<renderer_t> renderer,
    size_t const w, size_t const h,
	std::string output_fn
) noexcept :
    image_plane{std::make_unique<rgb_t<float>[]>(w * h)},
    renderer{std::move(renderer)}, output_fn{std::move(output_fn)}, width{w}, height{h} {}

image_t::~image_t(){}
