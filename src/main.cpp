//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include <memory>

#include "camera/camera.hpp"
#include "image/image.hpp"
#include "scene/scene.hpp"
#include "camera/perspective.hpp"
#include "renderer/renderer.hpp"

int main(){

    std::unique_ptr<scene::scene_t> const scene {std::make_unique<scene::scene_t>()};
    std::unique_ptr<cam::camera_t> const cam {std::make_unique<cam::perspective_t>(640, 480)};
    std::unique_ptr<img::image_t> const img {std::make_unique<img::image_t>()};

    bool const success {scene->load("models/cornell_box.obj")};

    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }

    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene->print_summary();
    std::cout << std::endl;

    // declare the renderer
    render::renderer_t renderer {cam.get(), scene.get(), img.get()};
    // render
    renderer.render();
    // save the image

    img->save("djfjdhfkj");


    std::cout << "That's all, folks!\n";

    return 0;
}
