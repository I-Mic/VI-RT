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

    std::unique_ptr<rgb::rgb_t<unsigned char>[]> const image_to_save;

    void tone_map() const noexcept;

public:

    image_ppm_t(
		std::unique_ptr<render::renderer_t> renderer,
		size_t const w, size_t const h,
		std::string output_fn
	);

    bool output_image() const override;
};

};

#endif /* IMAGE_PPM_HPP */
