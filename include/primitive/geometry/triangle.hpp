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
		vec::vec3_t v1, v2, v3;
		vec::vec3_t normal, edge1, edge2;
		prim::bb_t bb;

		std::optional<ray::intersection_t> intersect(ray::ray_t const& r) const override;

		triangle_t(vec::vec3_t _v1, vec::vec3_t _v2, vec::vec3_t _v3, vec::vec3_t _normal) :
			v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
			edge1 = (v2-v1); edge2 = (v3-v1);
			bb = { v1, v2, v3 };
		}
		// https://www.mathopenref.com/heronsformula.html
		float area();
	};
}
#endif
