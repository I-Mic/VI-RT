//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include <memory>

#include "camera/perspective.hpp"
#include "image/image_ppm.hpp"
#include "light/ambient_light.hpp"
#include "light/light.hpp"
#include "scene/scene.hpp"
#include "renderer/standard_renderer.hpp"
#include "shader/ambient_shader.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

static size_t constexpr width  {640};
static size_t constexpr height {480};

static std::unique_ptr<cam::camera_t> get_camera(){

    static vec::vec3_t const eye {3.f, 3.f, 3.f};
    static vec::vec3_t const at  {0.f, 0.f, 0.f};
    static vec::vec3_t const up  {0.f, 1.f, 0.f};
    static float const fov_w {3.14/3.f};
    static float const fov_h {3.14/3.f};

    std::unique_ptr<cam::camera_t> cam {
        std::make_unique<cam::perspective_t>(eye, at, up, width, height, fov_w, fov_h)
    };

    return cam;
}

static std::vector<std::unique_ptr<light::light_t>> get_lights(){

    std::vector<std::unique_ptr<light::light_t>> v {1};

    v.push_back(std::make_unique<light::ambient_light_t>(rgb::rgb_t<float>{0.9f, 0.9f, 0.9f}));

    return v;
}

static std::unique_ptr<shader::shader_t> get_shader(std::unique_ptr<scene::scene_t> scene){

    static rgb::rgb_t<float> const background {0.4f, 0.4f, 0.4f};

    std::unique_ptr<shader::shader_t> shader {
        std::make_unique<shader::ambient_shader_t>(std::move(scene), background)
    };

    return shader;
}


int main(int const argc, char const* const* const argv){

    if(argc < 3){
        std::cerr << "usage: " << argv[0] << " <INPUT_FILE> <OUTPUT_FILE>\n";
        return 1;
    }

    for(int i = 3; i < argc; ++i)
        std::cerr << argv[0] << ": unknown operand '" << argv[i] << "'\n";

    std::string const input_fn  {argv[1]};
    std::string const output_fn {argv[2]};


    std::unique_ptr<scene::scene_t> scene {
         std::make_unique<scene::scene_t>(input_fn, get_lights())
    };
    if(!scene->is_loaded()){
        std::cerr << argv[0] << ": error loading file '" << input_fn << "'\n";
        return 2;
    }
    else {
        std::cout << "scene loaded successfully\n";
        scene->print_summary();
        std::cout << '\n';
    }


    // declare the renderer
    std::unique_ptr<render::renderer_t> renderer {
        std::make_unique<render::standard_renderer_t>(
            get_camera(),
            get_shader(std::move(scene))
        )
    };

    std::unique_ptr<img::image_t> const img {
        std::make_unique<img::image_ppm_t>(
            std::move(renderer),
            width,
            height
        )
    };

    // produce the output
    if(!img->output_image(output_fn)){
        std::cerr << argv[0] << ": error outputting to file '" << output_fn << "'\n";
        return 2;
    }

    std::cout << "That's all, folks!\n";

    return 0;
}
