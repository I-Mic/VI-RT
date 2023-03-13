//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "primitive/primitive.hpp"
#include "light/light.hpp"
#include "rays/ray.hpp"
#include "rays/intersection.hpp"
#include "primitive/brdf/brdf.hpp"

namespace scene {

class scene_t {

private:

    std::vector<prim::primitive_t> prims;
    std::vector<light::light_t> lights;
    std::vector<std::unique_ptr<prim::brdf::brdf_t>> brdfs;

public:

    scene_t() noexcept;

    bool load(const std::string &fname);
    bool set_lights() noexcept;
    std::optional<ray::intersection_t> trace(ray::ray_t const& r) const;

    void print_summary() const;
};

};

#endif /* SCENE_HPP */
