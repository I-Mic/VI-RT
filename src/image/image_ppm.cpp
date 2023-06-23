//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include <fstream>
#include <memory>
#include <numeric>
#include <thread>

#include "image/image_ppm.hpp"
#include "renderer/renderer.hpp"
#include "utils/rgb.hpp"

image_ppm_t::image_ppm_t(
    std::unique_ptr<renderer_t> renderer,
    size_t const w, size_t const h,
    std::string output_fn,
    unsigned const num_of_threads,
    bool const normalize
) :
    image_t{std::move(renderer), w, h, std::move(output_fn)},
    image_to_save{std::make_unique<rgb_t<unsigned char>[]>(w * h)},
    num_of_threads{num_of_threads},
    normalize{normalize} {}

void image_ppm_t::clamp_tone_map() const {

    // loop over each pixel in the image, clamp and convert to byte format
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            rgb_t<float>&       from {this->image_plane[y * this->width + x]};
            rgb_t<unsigned char>& to {this->image_to_save[y * this->width + x]};

            to.r = static_cast<unsigned char>(std::min(1.f, from.r) * 255.f);
            to.g = static_cast<unsigned char>(std::min(1.f, from.g) * 255.f);
            to.b = static_cast<unsigned char>(std::min(1.f, from.b) * 255.f);
        }
}

void image_ppm_t::normalize_tone_map() const {

    static std::pair<float, float> const identity {
        std::make_pair<float, float>(
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::min()
        )
    };

    auto const [comp_min, comp_max] {
        std::accumulate(
            this->image_plane.get(),
            this->image_plane.get() + this->width * this->height,
            identity,
            [](
                std::pair<float, float> const& acc,
                rgb_t<float> const& item
            ) -> std::pair<float, float> {
                return
                    std::pair<float, float>{
                        std::min(std::min(std::min(acc.first, item.r), item.g), item.b),
                        std::max(std::max(std::max(acc.second, item.r), item.g), item.b)
                    };
            }
        )
    };

    float const comp_range {comp_max - comp_min};

    // loop over each pixel in the image, normalize and convert to byte format
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            rgb_t<float>&       from {this->image_plane[y * this->width + x]};
            rgb_t<unsigned char>& to {this->image_to_save[y * this->width + x]};

            to.r = static_cast<unsigned char>((from.r - comp_min) / comp_range * 255.f);
            to.g = static_cast<unsigned char>((from.g - comp_min) / comp_range * 255.f);
            to.b = static_cast<unsigned char>((from.b - comp_min) / comp_range * 255.f);
        }
}

void image_ppm_t::tone_map() const {
    if(this->normalize)
        this->normalize_tone_map();
    else
        this->clamp_tone_map();
}

void image_ppm_t::shade_pixels() const {

    auto const worker {

        [this](size_t const begin_index, size_t const end_index, rgb_t<float>* const buffer){

            size_t x {begin_index % this->width};
            size_t y {begin_index / this->width};

            size_t const local_size {end_index - begin_index};

            for(size_t i {0}; i < local_size; ++i){

                if(x >= this->width){
                    x = 0;
                    ++y;
                }

                rgb_t<float> const color {this->renderer->render_pixel(x, y)};
                buffer[i] = color;

                ++x;
            }
        }
    };


    size_t const image_plane_size {this->width * this->height};
    size_t const local_size {
        static_cast<size_t>(
            std::ceil(
                static_cast<double>(image_plane_size) /
                static_cast<double>(this->num_of_threads)
            )
        )
    };

    std::vector<std::thread> threads {};
    // better than calling constructor with argument = this->num_of_threads directly
    // this avoids default constructing objects that would be reassigned afterwards
    threads.reserve(this->num_of_threads);

    for(size_t lower_bound {0}; lower_bound < image_plane_size; lower_bound += local_size)
        threads.emplace_back(
            worker,
            lower_bound,
            std::min(lower_bound + local_size, image_plane_size),
            this->image_plane.get() + lower_bound
        );

    for(std::thread& t : threads)
        t.join();
}

bool image_ppm_t::output_image() const {

    //shade each pixel
    this->shade_pixels();

    this->tone_map();

    std::ofstream ofs {};
    ofs.open(this->output_fn, std::ios::out | std::ios::binary);
    if(ofs.fail())
        return false;

    ofs << "P6\n" << this->width << " " << this->height << "\n255\n";

    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x)

            ofs << this->image_to_save[y * this->width + x].r
                << this->image_to_save[y * this->width + x].g
                << this->image_to_save[y * this->width + x].b;

    return true;
}
