//
//  camera.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef camera_hpp
#define camera_hpp

// based on pbrt book, sec 6.1, pag. 356
class Camera {
public:
    Camera () {}
    ~Camera() {}
    bool GenerateRay() {return false;};
};

#endif /* camera_hpp */
