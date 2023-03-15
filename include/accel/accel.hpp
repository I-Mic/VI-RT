//
//  VI-RT
//
//  Created by Luis Paulo Santos on 31/01/2023.
//

#ifndef ACCEL_HPP
#define ACCEL_HPP

#include "rays/ray.hpp"
#include "rays/intersection.hpp"
#include "scene/scene.hpp"


namespace accel {

class accel_t {

public:

    accel_t();

    void build(scene::scene_t const& s);

    bool trace(scene::scene_t const& s, ray::ray_t const& r, ray::intersection_t const& isect);
};

};


#endif /* ACCEL_HPP */
