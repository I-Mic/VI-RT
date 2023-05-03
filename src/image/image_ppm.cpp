//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include <iostream>
#include <fstream>
#include <optional>
#include <thread>
#include <functional>

#include "image/image_ppm.hpp"
#include "renderer/renderer.hpp"
#include "utils/rgb.hpp"

namespace img {

image_ppm_t::image_ppm_t(
    std::unique_ptr<render::renderer_t> renderer,
    size_t const w, size_t const h,
    std::string output_fn,
    unsigned const num_of_threads
) :
    image_t{std::move(renderer), w, h, std::move(output_fn)},
    image_to_save{std::make_unique<rgb::rgb_t<unsigned char>[]>(w * h)},
    num_of_threads{num_of_threads} {}


void image_ppm_t::tone_map() const noexcept {

    // loop over each pixel in the image, clamp and convert to byte format
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            rgb::rgb_t<float>&       from {this->image_plane[y * this->width + x]};
            rgb::rgb_t<unsigned char>& to {this->image_to_save[y * this->width + x]};

            to.r = static_cast<unsigned char>(std::min(1.f, from.r) * 255.f);
            to.g = static_cast<unsigned char>(std::min(1.f, from.g) * 255.f);
            to.b = static_cast<unsigned char>(std::min(1.f, from.b) * 255.f);
        }
}

void image_ppm_t::shade_pixels() const {

    std::function<void(size_t, size_t)> const worker {

        [this] (size_t const begin_index, size_t const end_index) {

            size_t x {begin_index % this->width};
            size_t y {begin_index / this->width};

            for(size_t i {begin_index}; i < end_index; ++i){

                if(x >= this->width){
                    x = 0;
                    ++y;
                }
                if(y >= this->height)
                    return;

                rgb::rgb_t<float> const color {this->renderer->render_pixel(x, y)};
                this->image_plane[i] = color;

                ++x;
            }
        }
    };


    size_t const image_plane_size {this->width * this->height};
    size_t const local_size {
        static_cast<size_t>(
            std::ceil(
                static_cast<size_t>(image_plane_size) /
                static_cast<size_t>(num_of_threads)
            )
        )
    };

    std::vector<std::thread> threads {};
    threads.reserve(this->num_of_threads);
    for(size_t i {0}; i < this->num_of_threads; ++i)
        threads.emplace_back(
            worker, i * local_size,
            std::min((i + 1) * local_size, image_plane_size)
        );

    for(std::thread& t : threads)
        t.join();
}

bool image_ppm_t::output_image() const {

    //shade each pixel
    this->shade_pixels();

    // convert from float to {0,1,..., 255}
    this->tone_map();

    std::ofstream ofs {};
    ofs.open(this->output_fn, std::ios::out | std::ios::binary);
    if(ofs.fail())
        return false;


    ofs << "P6\n" << this->width << " " << this->height << "\n255\n";

    // loop over each pixel in the image, clamp and convert to byte format
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x)

            ofs << this->image_to_save[y * this->width + x].r
                << this->image_to_save[y * this->width + x].g
                << this->image_to_save[y * this->width + x].b;

    return true;
}

};
