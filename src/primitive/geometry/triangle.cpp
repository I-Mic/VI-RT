#include "primitive/geometry/triangle.hpp"
#include "utils/math_extra.hpp"

triangle_t::triangle_t(
    vec3_t const& v1,
    vec3_t const& v2,
    vec3_t const& v3,
    vec3_t const& normal
) noexcept :
    v1{v1}, v2{v2}, v3{v3},
    edge1{v2 - v1}, edge2{v3 - v1}, edge3{v2 - v3},
    normal{normal}, bb{v1, v2, v3} {}


std::optional<intersection_t> triangle_t::intersect(ray_t const& r) const noexcept {

    if(!this->bb.intersects(r))
        return std::nullopt;

    vec3_t const h {r.dir.cross(this->edge2)};
    float const a {this->edge1.dot(h)};

    if(a > -emath::EPSILON && a < emath::EPSILON)
        return std::nullopt;    // This ray is parallel to this triangle.


    float const f {1.f / a};
    vec3_t const s {r.org - this->v1};
    float const u {f * s.dot(h)};
    if(u < 0.f || u > 1.f)
        return std::nullopt;


    vec3_t const q {s.cross(this->edge1)};
    float const v {f * r.dir.dot(q)};
    if(v < 0.f || u + v > 1.f)
        return std::nullopt;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float const t {f * this->edge2.dot(q)};
    if(t > emath::EPSILON){

        vec3_t const wo {-1.f * r.dir};

        intersection_t const inter {
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
