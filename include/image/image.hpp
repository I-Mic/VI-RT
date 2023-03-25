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


namespace img {

class image_t {

protected:

    std::unique_ptr<rgb::rgb_t<float>[]> const image_plane;
    std::unique_ptr<render::renderer_t> const renderer;

public:

    size_t const width;
    size_t const height;

    image_t(std::unique_ptr<render::renderer_t> renderer, size_t const w, size_t const h) noexcept;
    virtual ~image_t();

    virtual bool output_image(std::string const& filename) const = 0;
};

};

#endif /* IMAGE_HPP */
