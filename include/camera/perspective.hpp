//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

#include <tuple>
#include <optional>
#include <array>

#include "camera/camera.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"


namespace cam {

class perspective_t : public camera_t {

private:
    vec::vec3_t eye;
    vec::vec3_t at;
    vec::vec3_t up;
    float field_of_view_w;
    float field_of_view_h;
    size_t const width;
    size_t const height;
    std::array<std::array<float, 3>, 3> c2w;

public:
    perspective_t(size_t const w, size_t const h); 

    std::optional<ray::ray_t> generate_ray(const int x, const int y, const float *cam_jitter=nullptr) override;

    std::tuple<size_t, size_t> get_resolution() const noexcept {
        return std::make_tuple(this->width, this->height);
    }
};

};

#endif /* PERSPECTIVE_HPP */
