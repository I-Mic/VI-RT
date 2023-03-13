//
//  VI-RT
//
//  Created by Luis Paulo Santos on 04/02/2023.
//

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "utils/vector.hpp"
#include "utils/rgb.hpp"

namespace light {

class light_t {

public:

    light_t () {}

    virtual ~light_t () {}

    // return the light_t RGB radiance for a given point : p
    virtual rgb::rgb_t L(vec::vec3_t p){ return rgb::rgb_t();}

    // return a point p and rgb::rgb_t radiance for a given probability pair prob[2]
    virtual rgb::rgb_t Sample_L (float *prob, vec::vec3_t *p) {return rgb::rgb_t();}

    // return the probability of p
    virtual float pdf(vec::vec3_t p) {return 0.;}
};

};

#endif /* LIGHT_HPP */
