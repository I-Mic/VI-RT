#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "primitive/geometry/mesh.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

class triangle_t : public geometry_t {

public:
    vec3_t const v1, v2, v3;
    vec3_t const edge1, edge2, edge3;
    vec3_t const normal;
    bb_t const bb;

    triangle_t(
        vec3_t const& v1,
        vec3_t const& v2,
        vec3_t const& v3,
        vec3_t const& normal
    ) noexcept;

    ~triangle_t() noexcept;

    std::optional<intersection_t> intersect(ray_t const& r) const override;
    float area() const noexcept;
};

#endif //TRIANGLE_HPP
