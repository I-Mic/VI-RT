//
//  VI-RT
//
//  Created by Luis Paulo Santos on 31/01/2023.
//

#include "accel/accel.hpp"

namespace accel {

accel_t::accel_t(){}

void accel_t::build(scene::scene_t const& s){}

bool accel_t::trace(scene::scene_t const& s, ray::ray_t const& r, ray::intersection_t const& isect){
    return false;
}

};
