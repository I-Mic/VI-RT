

#include "primitive.hpp"

Primitive::Primitive(Geometry *g, int material_ndx): g(g), material_ndx(material_ndx){}

Primitive::~Primitive() { delete g; }

Primitive::Primitive(Primitive&& p) : g(p.g), material_ndx(p.material_ndx) { p.g = nullptr; }