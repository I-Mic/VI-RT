//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include <memory>

#include "camera/camera.hpp"
#include "image/image.hpp"
#include "image/image_ppm.hpp"
#include "scene/scene.hpp"
#include "camera/perspective.hpp"
#include "renderer/renderer.hpp"
#include "renderer/standard_renderer.hpp"
#include "shader/shader.hpp"
#include "shader/ambient_shader.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

int main(){

    std::shared_ptr<scene::scene_t> const scene {
        std::make_shared<scene::scene_t>("models/cornell_box.obj")
    };
    if(!scene->is_loaded()) {
        std::cerr << "ERROR!! :o\n";
        return 1;
    }

    vec::vec3_t const eye {1.f, 1.f, 1.f};
    vec::vec3_t const at  {1.f, 1.f, 1.f};
    vec::vec3_t const up  {1.f, 1.f, 1.f};
    size_t const width  {640};
    size_t const height {480};
    float const fov_w {90.f};
    float const fov_h {90.f};
    std::shared_ptr<cam::camera_t> const cam {
        std::make_shared<cam::perspective_t>(eye, at, up, width, height, fov_w, fov_h)
    };

    std::shared_ptr<img::image_t> const img {
        std::make_shared<img::image_ppm_t>(width, height)
    };

    rgb::rgb_t<float> const background { 1.f, 1.f, 1.f };
    std::shared_ptr<shader::shader_t> const shader {
        std::make_shared<shader::ambient_shader_t>(scene, background)
    };


    std::cerr << "Scene Load: SUCCESS!! :-)\n";
    scene->print_summary();
    std::cout << '\n';

    // declare the renderer
    std::unique_ptr<render::renderer_t> const renderer {
        std::make_unique<render::standard_renderer_t>(cam, scene, img, shader)
    };

    // render
    renderer->render();

    // save the image
    img->save("djfjdhfkj");

    std::cout << "That's all, folks!\n";

    return 0;
}
