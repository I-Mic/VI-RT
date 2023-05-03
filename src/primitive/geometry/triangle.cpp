#include "primitive/geometry/triangle.hpp"

namespace prim::geo {

triangle_t::triangle_t(
    vec::vec3_t const& v1,
    vec::vec3_t const& v2,
    vec::vec3_t const& v3,
    vec::vec3_t const& normal
) noexcept :
    v1{v1}, v2{v2}, v3{v3},
    edge1{v2 - v1}, edge2{v3 - v2}, edge3{v1 - v3},
    normal{normal}, bb{v1, v2, v3} {}

triangle_t::~triangle_t() noexcept {}


std::optional<ray::intersection_t> triangle_t::intersect(ray::ray_t const& r) const {

    static float constexpr EPSILON {0.0000001f};

    if(!this->bb.intersects(r))
        return std::nullopt;

    vec::vec3_t const h {r.dir.cross_product(this->edge2)};
    float const a {this->edge1.dot_product(h)};

    if(a > -EPSILON && a < EPSILON)
        return std::nullopt;    // This ray is parallel to this triangle.


    float const f {1.f / a};
    vec::vec3_t const s {r.org - this->v1};
    float const u {f * s.dot_product(h)};
    if(u < 0.f || u > 1.f)
        return std::nullopt;


    vec::vec3_t const q {s.cross_product(this->edge1)};
    float const v {f * r.dir.dot_product(q)};
    if(v < 0.f || u + v > 1.f)
        return std::nullopt;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float const t {f * this->edge2.dot_product(q)};
    if(t > EPSILON){

        vec::vec3_t const wo {-1.f * r.dir};

        ray::intersection_t const inter {
            r.org + r.dir * t,
            this->normal.flip(wo),
            wo,
            t
       };

        return std::make_optional(inter);
   }

    // This means that there is a line intersection but not a ray intersection.
    return std::nullopt;
};

float triangle_t::area() const noexcept {
    float const norm_edge1 {this->edge1.norm()};
    float const norm_edge2 {this->edge2.norm()};
    float const norm_edge3 {this->edge3.norm()};
    float const p {(norm_edge1 + norm_edge2 + norm_edge3) / 2.f};

    return std::sqrt(p * (p - norm_edge1) * (p - norm_edge2) * (p - norm_edge3));
}

}


