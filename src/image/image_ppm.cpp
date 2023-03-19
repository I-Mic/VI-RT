//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include <iostream>
#include <fstream>
#include <optional>

#include "image/image_ppm.hpp"
#include "renderer/renderer.hpp"
#include "utils/rgb.hpp"

namespace img {

image_ppm_t::image_ppm_t(
    std::unique_ptr<render::renderer_t> render,
    size_t const w, size_t const h
) :
    image_t{std::move(render), w, h},
    image_to_save{std::make_unique<rgb::rgb_t<unsigned char>[]>(w * h)}{}


void image_ppm_t::tone_map() const noexcept {

    // loop over each pixel in the image, clamp and convert to byte format
    for(size_t j {0}; j < this->height; ++j)

        for(size_t i {0}; i < this->width; ++i){

            rgb::rgb_t<float>&       from {this->image_plane[j * this->width + i]};
            rgb::rgb_t<unsigned char>& to {this->image_to_save[j * this->width + i]};

            to.r = static_cast<unsigned char>(std::min(1.f, from.r) * 255);
            to.g = static_cast<unsigned char>(std::min(1.f, from.g) * 255);
            to.b = static_cast<unsigned char>(std::min(1.f, from.b) * 255);
        }
}

bool image_ppm_t::output_image(std::string const& filename) const {

    /*if(this->height == 0 || this->width == 0){
        std::cerr << "Can't save an empty image\n";
        return false;
    }*/

    //shade each pixel
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            std::optional<rgb::rgb_t<float>> const color {this->renderer->render_pixel(x, y)};

            if(color.has_value())
                this->image_plane[y * this->width + x] = color.value();
        }

    // convert from float to {0,1,..., 255}
    this->tone_map();

    std::ofstream ofs {};
    ofs.open(filename, std::ios::out | std::ios::binary);
    if(ofs.fail())
        return false;


    ofs << "P6\n" << this->width << " " << this->height << "\n255\n";

    // loop over each pixel in the image, clamp and convert to byte format
    for (size_t i {0}; i < this->width * this->height; ++i)
        ofs << this->image_to_save[i].r
            << this->image_to_save[i].g
            << this->image_to_save[i].b;

    return true;
}

};
