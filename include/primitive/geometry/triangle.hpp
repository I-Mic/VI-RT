#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "primitive/geometry/mesh.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

namespace prim::geo {

class triangle_t : public geometry_t {

public:
    vec::vec3_t const v1, v2, v3;
    vec::vec3_t const edge1, edge2, edge3;
    vec::vec3_t const normal;
    prim::bb_t const bb;

    triangle_t(
        vec::vec3_t const& v1,
        vec::vec3_t const& v2,
        vec::vec3_t const& v3,
        vec::vec3_t const& normal
    ) noexcept;

    ~triangle_t() noexcept;

    std::optional<ray::intersection_t> intersect(ray::ray_t const& r) const override;
    float area() const noexcept;
};

}

#endif //TRIANGLE_HPP
