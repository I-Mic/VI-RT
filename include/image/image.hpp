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

    std::unique_ptr<rgb::rgb_t[]> image_plane;
    size_t width, height;

public:

    image_t(): image_plane{}, width{0}, height{} {}

    image_t(const size_t w, const size_t h) noexcept :
        image_plane{std::make_unique<rgb::rgb_t[]>(w * h)},
        width{w},
        height{h} {}

    virtual ~image_t();

    bool set_at(size_t const x, size_t const y, rgb::rgb_t const& rgb) noexcept {

        if(x > this->width || y > this->height)
            return false;

        this->image_plane[y * this->width + x] = rgb;
        return true;
    }

    bool add_at(size_t const x, size_t const y, rgb::rgb_t const& rgb) {

        if(x > this->width || y > this->height)
            return false;

        this->image_plane[y * this->width + x] += rgb;
        return true;
    }

    bool save(std::string const& filename) const {
        return false;
    }
};

};

#endif /* IMAGE_HPP */
