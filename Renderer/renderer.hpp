//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef renderer_hpp
#define renderer_hpp

#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
    Image * img;
public:
    Renderer (Camera *cam, Scene * scene, Image * img): cam(cam), scene(scene), img(img) {}
    void Render () {}
};

#endif /* renderer_hpp */
