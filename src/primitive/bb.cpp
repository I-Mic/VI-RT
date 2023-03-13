#include "primitive/bb.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"
#include <limits>
#include <utility>

namespace prim {

bb_t::bb_t(vec::vec3_t const& p1) : min(p1), max(p1) {}

bb_t::bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2) : 
    min(
        { 
            std::min(p1.x, p2.x),
            std::min(p1.y, p2.y),
            std::min(p1.z, p2.z)
        }
    ),
    max(
        { 
            std::max(p1.x, p2.x),
            std::max(p1.y, p2.y),
            std::max(p1.z, p2.z)
        }
    ){}

bb_t::bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2, vec::vec3_t const& p3) : 
    min(
        { 
            std::min(std::min(p1.x, p2.x), p3.x),
            std::min(std::min(p1.y, p2.y), p3.y),
            std::min(std::min(p1.z, p2.z), p3.z)
        }
    ),
    max(
        { 
            std::max(std::max(p1.x, p2.x), p3.x),
            std::max(std::max(p1.y, p2.y), p3.y),
            std::max(std::max(p1.z, p2.z), p3.z)
        }
    ){}

bb_t bb_t::from_union_of(bb_t const& b1, bb_t const& b2){

    return {
        {
            std::min(b1.min.x, b2.min.x),
            std::min(b1.min.y, b2.min.y),
            std::min(b1.min.z, b2.min.z),
        },
        {
            std::max(b1.max.x, b2.max.x),
            std::max(b1.max.y, b2.max.y),
            std::max(b1.max.z, b2.max.z),
        }
    };
}




bool bb_t::intersects(ray::ray_t const& r) const {

    float t0 = 0;
    float t1 = std::numeric_limits<float>::max();


    float const inv_ray_dir_x = 1 / r.dir.x;
    float const inv_ray_dir_y = 1 / r.dir.y;
    float const inv_ray_dir_z = 1 / r.dir.z;

    float t_near_x = (this->min.x - r.o.x) * inv_ray_dir_x;
    float t_far_x  = (this->max.x - r.o.x) * inv_ray_dir_x;

    if(t_near_x > t_far_x)
        std::swap(t_near_x, t_far_x);

    //t_far_x *= 1 + 2 * gamma(3);

    t0 = t_near_x > t0 ? t_near_x : t0;
    t1 = t_far_x  < t1 ? t_far_x  : t1;

    if (t0 > t1) 
        return false;


    float t_near_y = (this->min.y - r.o.y) * inv_ray_dir_y;
    float t_far_y  = (this->max.y - r.o.y) * inv_ray_dir_y;

    if(t_near_y > t_far_y)
        std::swap(t_near_y, t_far_y);

    //t_far_y *= 1 + 2 * gamma(3);

    t0 = t_near_y > t0 ? t_near_y : t0;
    t1 = t_far_y  < t1 ? t_far_y  : t1;

    if (t0 > t1) 
        return false;


    float t_near_z = (this->min.z - r.o.z) * inv_ray_dir_z;
    float t_far_z  = (this->max.z - r.o.z) * inv_ray_dir_z;

    if(t_near_z > t_far_z)
        std::swap(t_near_z, t_far_z);

    //t_far_z *= 1 + 2 * gamma(3);

    t0 = t_near_z > t0 ? t_near_z : t0;
    t1 = t_far_z  < t1 ? t_far_z  : t1;

    if (t0 > t1) 
        return false;

    return true;
}

};
