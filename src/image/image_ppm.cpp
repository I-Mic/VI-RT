//
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include <fstream>
#include <memory>
#include <numeric>
#include <thread>
#include <cstring>

#include "image/image_ppm.hpp"
#include "renderer/renderer.hpp"
#include "utils/rgb.hpp"

namespace img {

image_ppm_t::image_ppm_t(
    std::unique_ptr<render::renderer_t> renderer,
    size_t const w, size_t const h,
    std::string output_fn,
    unsigned const num_of_threads,
    bool const normalize
) :
    image_t{std::move(renderer), w, h, std::move(output_fn)},
    image_to_save{std::make_unique<rgb::rgb_t<unsigned char>[]>(w * h)},
    num_of_threads{num_of_threads},
    normalize{normalize} {}

void image_ppm_t::clamp_tone_map() const {

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

/*void image_ppm_t::hsv_tone_map() const {

    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            rgb::rgb_t<float>&         from {this->image_plane[y * this->width + x]};
            rgb::rgb_t<unsigned char>& to {this->image_to_save[y * this->width + x]};

            float const cmax {std::max(std::max(from.r, from.g), from.b)};
            float const cmin {std::min(std::min(from.r, from.g), from.b)};
            float const d {cmax - cmin};

            float const hue {
                d == 0.f
                    ? 0.f
                    : (cmax == from.r
                        ? (static_cast<int>(60.f * (from.g - from.b) / d + 360.f) % 360)
                        : (cmax == from.g
                            ? (static_cast<int>(60.f * (from.b - from.r) / d + 120.f) % 360)
                            : (static_cast<int>(60.f * (from.r - from.g) / d + 240.f) % 360)
                        )
                    )
            };
            float const saturation {cmax == 0.f ? 0.f : d / cmax};
            float const value {cmax};


            float const c {(saturation > 1.f ? 0.9f : saturation) * value};
            float const t {c * (1.f - std::abs(-1.f + static_cast<int>(hue / 60.f) % 2))};

            if(hue < 60.f){
                from = {c, t, 0.f};
            }
            else if(hue < 120.f){
                from = {t, c, 0.f};
            }
            else if(hue < 180.f){
                from = {0.f, c, t};
            }
            else if(hue < 240.f){
                from = {0.f, t, c};
            }
            else if(hue < 300.f){
                from = {t, 0.f, c};
            }
            else {
                from = {c, 0.f, t};
            }

            to.r = static_cast<unsigned char>(std::min(1.f, from.r) * 255.f);
            to.g = static_cast<unsigned char>(std::min(1.f, from.g) * 255.f);
            to.b = static_cast<unsigned char>(std::min(1.f, from.b) * 255.f);
        }
}*/

void image_ppm_t::normalize_tone_map() const {

    using rgb_pair_t = std::pair<rgb::rgb_t<float>, rgb::rgb_t<float>>;

    static float constexpr maxf {std::numeric_limits<float>::max()};
    static float constexpr minf {std::numeric_limits<float>::min()};
    static rgb_pair_t const identity {
        std::make_pair<rgb::rgb_t<float>, rgb::rgb_t<float>>(
            {maxf, maxf, maxf},
            {minf, minf, minf}
        )
    };

    auto const& [min_triplet, max_triplet] {

        std::accumulate(
            this->image_plane.get(),
            this->image_plane.get() + this->width * this->height,
            identity,
            [] (rgb_pair_t const& acc, rgb::rgb_t<float> const& item) -> rgb_pair_t {
                return
                    std::make_pair<rgb::rgb_t<float>, rgb::rgb_t<float>>(
                        {
                            std::min(acc.first.r, item.r),
                            std::min(acc.first.g, item.g),
                            std::min(acc.first.b, item.b)
                        },
                        {
                            std::max(acc.second.r, item.r),
                            std::max(acc.second.g, item.g),
                            std::max(acc.second.b, item.b)
                        }
                    );
            }
        )
    };

    float const r_range {max_triplet.r - min_triplet.r};
    float const g_range {max_triplet.g - min_triplet.g};
    float const b_range {max_triplet.b - min_triplet.b};

    // loop over each pixel in the image, normalize and convert to byte format
    for(size_t y {0}; y < this->height; ++y)

        for(size_t x {0}; x < this->width; ++x){

            rgb::rgb_t<float>&       from {this->image_plane[y * this->width + x]};
            rgb::rgb_t<unsigned char>& to {this->image_to_save[y * this->width + x]};

            to.r = static_cast<unsigned char>((from.r - min_triplet.r) / r_range * 255.f);
            to.g = static_cast<unsigned char>((from.g - min_triplet.g) / g_range * 255.f);
            to.b = static_cast<unsigned char>((from.b - min_triplet.b) / b_range * 255.f);
        }
}

void image_ppm_t::tone_map() const {
    if(this->normalize)
        this->normalize_tone_map();
        //this->hsv_tone_map();
    else
        this->clamp_tone_map();
}

void image_ppm_t::shade_pixels() const {

    auto const worker {

        [this] (size_t const begin_index, size_t const end_index, rgb::rgb_t<float>* const buffer){

            size_t x {begin_index % this->width};
            size_t y {begin_index / this->width};

            size_t const local_size {end_index - begin_index};

            // sanity check
            if(y >= this->height)
                return;

            for(size_t i {0}; i < local_size; ++i){

                if(x >= this->width){
                    x = 0;
                    ++y;
                    if(y >= this->height)
                        return;
                }

                rgb::rgb_t<float> const color {this->renderer->render_pixel(x, y)};
                buffer[i] = color;

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
    //std::vector<std::pair<std::unique_ptr<rgb::rgb_t<float>[]>, size_t>> buffers {};
    threads.reserve(this->num_of_threads);
    //buffers.reserve(this->num_of_threads);
    for(size_t i {0}; i < this->num_of_threads; ++i){

        size_t const lower_bound {i * local_size};
        size_t const upper_bound {std::min((i + 1) * local_size, image_plane_size)};

        //buffers.push_back(
        //    std::make_pair(
        //        std::make_unique<rgb::rgb_t<float>[]>(upper_bound - lower_bound),
        //        upper_bound - lower_bound
        //    )
        //);

        threads.emplace_back(
            worker,
            lower_bound,
            upper_bound,
            this->image_plane.get() + lower_bound //buffers.back().first.get()
        );
    }

    for(std::thread& t : threads)
        t.join();

    /*size_t offset {0};
    for(std::pair<std::unique_ptr<rgb::rgb_t<float>[]>, size_t> const& buffer : buffers){
        std::memcpy(
            this->image_plane.get() + offset,
            buffer.first.get(),
            buffer.second * sizeof(*this->image_plane.get())
        );
        offset += buffer.second;
    }*/
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
