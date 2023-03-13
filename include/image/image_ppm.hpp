//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#ifndef IMAGE_PPM_HPP
#define IMAGE_PPM_HPP


#include "image/image.hpp"
#include "utils/rgb.hpp"

namespace img {

class image_ppm_t : public image_t {

private:

    rgb::rgb_t *image_to_save;

    void tone_map();

public:

    image_ppm_t(size_t const w, size_t const h): image_t(w, h) {}

    bool save(std::string const& filename);
};

};

#endif /* IMAGE_PPM_HPP */
