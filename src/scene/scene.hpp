//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <string>
#include <vector>

#include "primitive/brdf/material.hpp"
#include "primitive/primitive.hpp"
#include "light/light.hpp"
#include "rays/ray.hpp"
#include "rays/intersection.hpp"

class scene_t {

private:

    bool success;
    std::vector<primitive_t> prims;
    std::vector<std::unique_ptr<light_t>> lights;
    std::vector<material_t> materials;

public:

    using lights_citer_t = std::vector<std::unique_ptr<light_t>>::const_iterator;

    scene_t(std::vector<std::unique_ptr<light_t>> lights) noexcept;
    scene_t(std::string const& fn, std::vector<std::unique_ptr<light_t>> lights);

    void load(std::string const& fn);
    bool is_loaded() const noexcept;

    std::pair<lights_citer_t, lights_citer_t> get_lights_iterator() const noexcept;
    material_t const* material_at(size_t const index) const noexcept;

    std::optional<intersection_t> trace(ray_t const& r) const noexcept;

    bool is_visible(ray_t const& r, float const max_l) const noexcept;

    void print_summary() const;
};

#endif /* SCENE_HPP */
