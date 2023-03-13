//
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <array>
#include <optional>

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

// partially inspired in pbrt book (3rd ed.), sec 3.6, pag 152

namespace prim::geo {

struct face_t {
    std::array<size_t, 3> vert_indices;
    vec::vec3_t geo_normal;           // geometric normal
    std::optional<std::array<size_t, 3>> vert_normals_indices;
    prim::bb_t bb;      // face bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
    face_t();
    bool has_shading_normals() const noexcept;
};


class mesh_t : public geo::geometry_t {

private:
    std::vector<face_t> faces;
    std::vector<vec::vec3_t> vertices;
    std::vector<vec::vec3_t> normals;

    std::optional<ray::intersection_t> triangle_intersect(ray::ray_t const& r, size_t const face_index) const;

public:
    mesh_t();
    mesh_t(
        std::vector<face_t> faces,
        std::vector<vec::vec3_t> vertices,
        std::vector<vec::vec3_t> normals,
        prim::bb_t b
    );

    std::optional<ray::intersection_t> intersect(ray::ray_t const& r) const override;
};

};

#endif /* MESH_HPP */
