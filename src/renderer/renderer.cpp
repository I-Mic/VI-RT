#include "renderer/renderer.hpp"


namespace render {

renderer_t::renderer_t(cam::camera_t* cam, scene::scene_t* scene, img::image_t* img) :
    cam(cam),
    scene(scene),
    img(img) {}


void renderer_t::render(){

}

};
