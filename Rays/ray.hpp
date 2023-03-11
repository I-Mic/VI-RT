//
//  Ray.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "vector.hpp"

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction
    Ray () {}
    Ray (Point o, Vector d): o(o),dir(d) {}
    ~Ray() {}
};

#endif /* Ray_hpp */
