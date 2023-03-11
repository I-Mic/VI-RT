//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "renderer.hpp"
#include "ImagePPM.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    bool success;
    
    success = scene.Load("C:/Users/novai/Downloads/CG/VI/projeto/VI-RT/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
    
    // Image resolution
    const int W= 640;
    const int H= 480;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={0,0,0}, At={0,0,1};
    const Vector Up={0,1,0};
    const float fovW = 3.14f/3.f, fovH = 3.14f/3.f;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);
    // declare the renderer
    Renderer myRender (cam, &scene, img);
    // render
    myRender.Render();
    // save the image
    
    img->Save("MyImage.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
