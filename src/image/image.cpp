#include "image/image.hpp"

namespace img {

image_t::image_t() noexcept :
    image_plane{}, width{0}, height{0} {}

image_t::image_t(size_t const w, size_t const h) noexcept :
    image_plane{std::make_unique<rgb::rgb_t<float>[]>(w * h)},
    width{w}, height{h} {}

image_t::~image_t(){}

bool image_t::set_at(size_t const x, size_t const y, rgb::rgb_t<float> const& rgb) const noexcept {

    if(x > this->width || y > this->height)
        return false;

    this->image_plane[y * this->width + x] = rgb;
    return true;
}

bool image_t::add_at(size_t const x, size_t const y, rgb::rgb_t<float> const& rgb) const noexcept {

    if(x > this->width || y > this->height)
        return false;

    this->image_plane[y * this->width + x] += rgb;
    return true;
}

};
