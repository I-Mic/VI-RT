//
//  primitive.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef primitive_hpp
#define primitive_hpp

#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"

struct Primitive {
    Geometry *g;
    int material_ndx;
    Primitive(Geometry *g, int material_ndx);
    ~Primitive();
    Primitive(Primitive && p);
};



#endif /* primitive_hpp */
