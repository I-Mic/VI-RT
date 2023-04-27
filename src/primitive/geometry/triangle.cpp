#include "primitive/geometry/triangle.hpp"

namespace prim::geo {

	std::optional<ray::intersection_t> triangle_t::intersect(ray::ray_t const& r) const {
        static float constexpr EPSILON{ 0.0000001f };

        if (!face.bb.intersects(r))
            return std::nullopt;

        vec::vec3_t const& v0{ this->vertices.at(face.vert_indices[0]) };
        vec::vec3_t const& v1{ this->vertices.at(face.vert_indices[1]) };
        vec::vec3_t const& v2{ this->vertices.at(face.vert_indices[2]) };

        vec::vec3_t const edge1{ v1 - v0 };
        vec::vec3_t const edge2{ v2 - v0 };

        vec::vec3_t const h{ r.dir.cross_product(edge2) };
        float const a{ edge1.dot_product(h) };

        if (a > -EPSILON && a < EPSILON)
            return std::nullopt;    // This ray is parallel to this triangle.


        float const f{ 1.f / a };
        vec::vec3_t const s{ r.org - v0 };
        float const u{ f * s.dot_product(h) };
        if (u < 0.f || u > 1.f)
            return std::nullopt;


        vec::vec3_t const q{ s.cross_product(edge1) };
        float const v{ f * r.dir.dot_product(q) };
        if (v < 0.f || u + v > 1.f)
            return std::nullopt;

        // At this stage we can compute t to find out where the intersection point is on the line.
        float const t{ f * edge2.dot_product(q) };
        if (t > EPSILON) {

            vec::vec3_t const wo{ -1.f * r.dir };

            ray::intersection_t const inter{
                r.org + r.dir * t,
                face.geo_normal.flip(wo),
                wo,
                t
            };

            return std::make_optional(inter);
        }

        // This means that there is a line intersection but not a ray intersection.
        return std::nullopt;
	};

    float area() {
        float edge_a = (v2 - v1).norm();
        float edge_b = (v3 - v2).norm();
        float edge_c = (v1 - v3).norm();
        float p = (edge_a + edge_b + edge_c) / 2;

        return sqrt(p * (p - edge_a) * (p - edge_b) * (p - edge_c));

    }


}


