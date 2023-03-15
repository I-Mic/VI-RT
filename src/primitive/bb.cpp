#include <limits>
#include <tuple>

#include "primitive/bb.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

namespace prim {

bb_t::bb_t() noexcept : min({0.f, 0.f, 0.f}), max({0.f, 0.f, 0.f}) {}

bb_t::bb_t(vec::vec3_t const& p1) noexcept : min(p1), max(p1) {}

bb_t::bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2) noexcept :
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

bb_t::bb_t(vec::vec3_t const& p1, vec::vec3_t const& p2, vec::vec3_t const& p3) noexcept :
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

bb_t bb_t::from_union_of(bb_t const& b1, bb_t const& b2) noexcept {

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

static bool get_t0_and_t1(
    float const ray_dir_cp, float const ray_org_cp,
    float const min_cp, float const max_cp,
    float& t0, float& t1
){

    float const inv_ray_dir_cp {1 / ray_dir_cp};
    float t_near_cp {(min_cp - ray_org_cp) * inv_ray_dir_cp};
    float t_far_cp  {(max_cp - ray_org_cp) * inv_ray_dir_cp};

    if(t_near_cp > t_far_cp)
        std::swap(t_near_cp, t_far_cp);

    //t_far_x *= 1 + 2 * gamma(3);

    t0 = t_near_cp > t0 ? t_near_cp : t0;
    t1 = t_far_cp  < t1 ? t_far_cp  : t1;

    return t0 <= t1;
}

bool bb_t::intersects(ray::ray_t const& r) const {

    float t0 {0.f};
    float t1 {std::numeric_limits<float>::max()};

    return
        get_t0_and_t1(r.dir.x, r.org.x, this->min.x, this->max.x, t0, t1) &&
        get_t0_and_t1(r.dir.y, r.org.y, this->min.y, this->max.y, t0, t1) &&
        get_t0_and_t1(r.dir.z, r.org.z, this->min.z, this->max.z, t0, t1);
}

};
