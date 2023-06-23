//
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <unordered_map>
#include <array>
#include <optional>

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

// partially inspired in pbrt book (3rd ed.), sec 3.6, pag 152

struct face_t {

    std::array<size_t, 3> vert_indices;
    vec3_t geo_normal;           // geometric normal
    std::optional<std::array<size_t, 3>> normals_indices;
    bb_t bb;      // face bounding box
    vec3_t edge1;
    vec3_t edge2;

    face_t() noexcept = default;
    bool has_shading_normals() const noexcept;
};


class mesh_t : public geometry_t {

private:
    std::vector<face_t> const faces;
    std::unordered_map<size_t, vec3_t> const vertices;
    std::unordered_map<size_t, vec3_t> const normals;

    std::optional<intersection_t> triangle_intersect(
        ray_t const& r,
        face_t const& face
    ) const noexcept;

public:
    mesh_t() noexcept = default;
    mesh_t(
        std::vector<face_t> faces,
        std::unordered_map<size_t, vec3_t> vertices,
        std::unordered_map<size_t, vec3_t> normals,
        bb_t b
    ) noexcept;

    ~mesh_t() noexcept = default;

    std::optional<intersection_t> intersect(ray_t const& r) const noexcept override;
};

#endif /* MESH_HPP */
