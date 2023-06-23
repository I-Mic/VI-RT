//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <memory>

#include "utils/rgb.hpp"
#include "renderer/renderer.hpp"


class image_t {

protected:

    std::unique_ptr<rgb_t<float>[]> const image_plane;
    std::unique_ptr<renderer_t> const renderer;
    std::string const output_fn;

public:

    size_t const width;
    size_t const height;

    image_t(
        std::unique_ptr<renderer_t> renderer,
        size_t const w, size_t const h,
        std::string output_fn
    ) noexcept :
        image_plane{std::make_unique<rgb_t<float>[]>(w * h)},
        renderer{std::move(renderer)}, output_fn{std::move(output_fn)}, width{w}, height{h} {}

    virtual ~image_t() noexcept = default;

    virtual bool output_image() const = 0;
};

#endif /* IMAGE_HPP */
