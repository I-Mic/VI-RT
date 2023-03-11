//
//  Scene.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>
#include "primitive.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "BRDF.hpp"

class Scene {
    int numPrimitives, numLights, numBRDFs;
    std::vector <Primitive> prims;
    std::vector <Light> lights;
    std::vector <BRDF> BRDFs;
public:
    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    void printSummary(void) {
        std::cout << "#primitives = " << prims.size() << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;
    }
};

#endif /* Scene_hpp */
