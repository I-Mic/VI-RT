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


namespace img {

class image_t {

protected:

    std::unique_ptr<rgb::rgb_t<float>[]> image_plane;

public:

    size_t const width;
    size_t const height;

    image_t() noexcept;
    image_t(size_t const w, size_t const h) noexcept;
    virtual ~image_t();

    bool set_at(size_t const x, size_t const y, rgb::rgb_t<float> const& rgb) const noexcept;
    bool add_at(size_t const x, size_t const y, rgb::rgb_t<float> const& rgb) const noexcept;
    virtual bool save(std::string const& filename) const = 0; 
};

};

#endif /* IMAGE_HPP */
