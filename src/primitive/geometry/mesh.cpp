//
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include <iostream>
#include <optional>
#include <tuple>
#include <limits>

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "primitive/geometry/mesh.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"
#include "utils/math_extra.hpp"

bool face_t::has_shading_normals() const noexcept {
    return this->normals_indices.has_value();
}


mesh_t::mesh_t(
    std::vector<face_t> faces,
    std::unordered_map<size_t, vec3_t> vertices,
    std::unordered_map<size_t, vec3_t> normals,
    bb_t b
) noexcept :
    geometry_t{b},
    faces{std::move(faces)},
    vertices{std::move(vertices)},
    normals{std::move(normals)} {}


std::optional<intersection_t> mesh_t::triangle_intersect(
    ray_t const& r, face_t const& face
) const noexcept {

    if(!face.bb.intersects(r))
        return std::nullopt;

    vec3_t const& v0 {this->vertices.at(face.vert_indices[0])};

    vec3_t const h {r.dir.cross(face.edge2)};
    float const a {face.edge1.dot(h)};

    if (a > -emath::EPSILON && a < emath::EPSILON)
        return std::nullopt;    // This ray is parallel to this triangle.


    float const f {1.f / a};
    vec3_t const s {r.org - v0};
    float const u {f * s.dot(h)};
    if (u < 0.f || u > 1.f)
        return std::nullopt;


    vec3_t const q {s.cross(face.edge1)};
    float const v {f * r.dir.dot(q)};
    if (v < 0.f || u + v > 1.f)
        return std::nullopt;


    // At this stage we can compute t to find out where the intersection point is on the line.
    float const t {f * face.edge2.dot(q)};
    if (t > emath::EPSILON){
        vec3_t const wo {-1.f * r.dir};
        intersection_t const inter {r.org + r.dir * t, face.geo_normal.flip(wo), wo, t};
        return std::make_optional(inter);
    }

    // This means that there is a line intersection but not a ray intersection.
    return std::nullopt;
}

std::optional<intersection_t> mesh_t::intersect(ray_t const& r) const noexcept {

    bool intersects {false};
    intersection_t min_isect {};
    min_isect.depth = std::numeric_limits<float>::max();

    // intersect the ray with the mesh BB
    if(!this->bb.intersects(r))
        return std::nullopt;

    // If it intersects then loop through the faces
    for(face_t const& face : this->faces){

        std::optional<intersection_t> inter {this->triangle_intersect(r, face)};
        if (!inter.has_value())
            continue;

        intersects = true;
        if (inter.value().depth < min_isect.depth) // this is closer
            min_isect = inter.value();
    }

    return intersects ? std::make_optional(min_isect) : std::nullopt;
}
