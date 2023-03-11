//
//  light.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 04/02/2023.
//

#ifndef light_hpp
#define light_hpp

#include "vector.hpp"
#include "RGB.hpp"

class Light {
public:
    Light () {}
    ~Light () {}
    // return the Light RGB radiance for a given point : p
    virtual RGB L (Point p) {return RGB();}
    // return a point p and RGB radiance for a given probability pair prob[2]
    virtual RGB Sample_L (float *prob, Point *p) {return RGB();}
    // return the probability of p
    virtual float pdf(Point p) {return 0.;}
};

#endif /* light_hpp */
