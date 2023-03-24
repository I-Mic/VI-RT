//
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include <optional>
#include <tuple>
#include <limits>

#include "primitive/bb.hpp"
#include "primitive/geometry/geometry.hpp"
#include "primitive/geometry/mesh.hpp"
#include "rays/intersection.hpp"
#include "rays/ray.hpp"
#include "utils/vector.hpp"

namespace prim::geo {

face_t::face_t() noexcept :
    vert_indices{}, geo_normal{},
    normals_indices{std::nullopt},
    bb{} {}

bool face_t::has_shading_normals() const noexcept {
    return this->normals_indices.has_value();
}



mesh_t::mesh_t() noexcept : faces{}, vertices{}, normals{} {}

mesh_t::mesh_t(
    std::vector<face_t> faces,
    std::unordered_map<size_t, vec::vec3_t> vertices,
	std::unordered_map<size_t, vec::vec3_t> normals,
    prim::bb_t b
) noexcept :
    geo::geometry_t{b},
    faces{std::move(faces)},
    vertices{std::move(vertices)},
    normals{std::move(normals)} {}

mesh_t::~mesh_t() noexcept {}


std::optional<ray::intersection_t> mesh_t::triangle_intersect(
    ray::ray_t const& r, face_t const& face
) const noexcept {

    static float constexpr EPSILON {0.0000001f};

    if(!face.bb.intersects(r))
        return std::nullopt;

    vec::vec3_t const& v0 {this->vertices.at(face.vert_indices[0])};
    vec::vec3_t const& v1 {this->vertices.at(face.vert_indices[1])};
    vec::vec3_t const& v2 {this->vertices.at(face.vert_indices[2])};

    vec::vec3_t const edge1 {v1 - v0};
    vec::vec3_t const edge2 {v2 - v0};

    vec::vec3_t const h {r.dir.cross_product(edge2)};
    float const a {edge1.dot_product(h)};

    if (a > -EPSILON && a < EPSILON)
        return std::nullopt;    // This ray is parallel to this triangle.


    float const f {1.f / a};
    vec::vec3_t const s {r.org - v0};
    float const u {f * s.dot_product(h)};
    if (u < 0.f || u > 1.f)
        return std::nullopt;


    vec::vec3_t const q {s.cross_product(edge1)};
    float const v {f * r.dir.dot_product(q)};
    if (v < 0.f || u + v > 1.f)
        return std::nullopt;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float const t {f * edge2.dot_product(q)};
    if (t > EPSILON){
        ray::intersection_t const inter {
            r.org + r.dir * t,
            face.geo_normal,
            {},
            t
        };

        return std::make_optional(inter);
    }

    // This means that there is a line intersection but not a ray intersection.
    return std::nullopt;
}

std::optional<ray::intersection_t> mesh_t::intersect(ray::ray_t const& r) const {

    bool intersects {false};
    ray::intersection_t min_isect {};
    min_isect.depth = std::numeric_limits<float>::max();

    // intersect the ray with the mesh BB
    if(!this->bb.intersects(r))
        return std::nullopt;

    // If it intersects then loop through the faces
    for(face_t const& face : this->faces){

        std::optional<ray::intersection_t> inter {this->triangle_intersect(r, face)};
        if (!inter.has_value())
            continue;

        intersects = true;
        if (inter.value().depth < min_isect.depth) // this is closer
            min_isect = inter.value();
    }

    return intersects ? std::make_optional(min_isect) : std::nullopt;
}

};
