//
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "camera/camera.hpp"
#include "image/image.hpp"
#include "scene/scene.hpp"

namespace render {

class renderer_t {
protected:
    cam::camera_t *cam;
    scene::scene_t *scene;
    img::image_t *img;
public:
    renderer_t(cam::camera_t* cam, scene::scene_t* scene, img::image_t* img);
    void render ();
};

};

#endif /* RENDERER_HPP */
