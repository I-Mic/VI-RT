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

#include "primitive/primitive.hpp"
#include "light/light.hpp"
#include "rays/ray.hpp"
#include "rays/intersection.hpp"
#include "primitive/brdf/brdf.hpp"

using lights_iter_t = std::vector<std::unique_ptr<light_t>>::const_iterator;
using brdfs_iter_t = std::vector<std::unique_ptr<brdf_t>>::const_iterator;

class scene_t {

private:

    bool success;
    std::vector<primitive_t> prims;
    std::vector<std::unique_ptr<light_t>> lights;
    std::vector<std::unique_ptr<brdf_t>> brdfs;


public:

    scene_t(std::vector<std::unique_ptr<light_t>> lights) noexcept;
    scene_t(std::string const& fn, std::vector<std::unique_ptr<light_t>> lights);

    void load(std::string const& fn);

    bool is_loaded() const noexcept;

    std::pair<lights_iter_t, lights_iter_t> get_lights_iterator() const noexcept;
    std::pair<brdfs_iter_t, brdfs_iter_t> get_brdfs_iterator() const noexcept;

    std::optional<intersection_t> trace(ray_t const& r) const noexcept;

	bool is_visible(ray_t const& r, float const max_l) const noexcept; 

    void print_summary() const;
};

#endif /* SCENE_HPP */
