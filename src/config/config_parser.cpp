#include <stdexcept>
#include <string>
#include <iostream>

#include "config/config_parser.hpp"
#include "camera/camera.hpp"
#include "camera/perspective.hpp"
#include "image/image.hpp"
#include "image/image_ppm.hpp"
#include "light/ambient_light.hpp"
#include "light/area_light.hpp"
#include "light/light.hpp"
#include "light/point_light.hpp"
#include "primitive/geometry/triangle.hpp"
#include "renderer/renderer.hpp"
#include "renderer/standard_renderer.hpp"
#include "scene/scene.hpp"
#include "shader/ambient_shader.hpp"
#include "shader/distributed_shader.hpp"
#include "shader/shader.hpp"
#include "shader/whitted_shader.hpp"
#include "toml.hpp"
#include "toml/exception.hpp"
#include "toml/get.hpp"
#include "toml/value.hpp"
#include "utils/vector.hpp"

namespace config {

static float to_rads(float const d) noexcept {
    return 3.1415f * d / 180.f;
}

config_parser_t::config_parser_t() noexcept {}

config_parser_t::~config_parser_t(){}

class toml_config_parser_t : public config_parser_t {

private:

    toml::value const toml_obj;
    bool const use_degrees;

    std::unique_ptr<cam::camera_t> build_camera() const override {

        std::string const table_name {"camera"};
        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};

        if(type == "perspective"){

            size_t const width {toml::find<size_t>(this->toml_obj, table_name, "width")};
            size_t const height {toml::find<size_t>(this->toml_obj, table_name, "height")};

            std::array<float, 3> const eye {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "eye")
            };

            std::array<float, 3> const at {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "at")
            };

            std::array<float, 3> const up {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "up")
            };

            float const fov_w {toml::find<float>(this->toml_obj, table_name, "fov_w")};
            float const fov_h {toml::find<float>(this->toml_obj, table_name, "fov_h")};

            return std::make_unique<cam::perspective_t>(
                vec::vec3_t::from_array(eye),
                vec::vec3_t::from_array(at),
                vec::vec3_t::from_array(up),
                width, height,
                this->use_degrees ? to_rads(fov_w) : fov_w,
                this->use_degrees ? to_rads(fov_h) : fov_h
            );
        }
        else
            throw std::domain_error("Unknown type " + type + " for asset " + table_name);
    }


    std::unique_ptr<scene::scene_t> build_scene() const override {

        std::string const table_name {"scene"};

        std::string const input_fn {
            toml::find<std::string>(this->toml_obj, table_name, "input_fn")
        };


        std::vector<std::unique_ptr<light::light_t>> lights {};
        std::vector<std::string> const light_ids {
            toml::find<std::vector<std::string>>(this->toml_obj, table_name, "light_ids")
        };

        for(std::string const& id : light_ids){

            std::string const type {
                toml::find<std::string>(this->toml_obj, table_name, id, "type")
            };

            if(type == "ambient"){

                std::array<float, 3> const color {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "color")
                };

                lights.push_back(
                    std::make_unique<light::ambient_light_t>(
                        rgb::rgb_t<float>::from_array(color)
                    )
                );
            }
            else if(type == "point"){

                std::array<float, 3> const color {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "color")
                };

                std::array<float, 3> const pos {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "pos")
                };

                lights.push_back(
                    std::make_unique<light::point_light_t>(
                        rgb::rgb_t<float>::from_array(color),
                        vec::vec3_t::from_array(pos)
                    )
                );
            }
            else if(type == "area"){

                std::array<float, 3> const power {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "power")
                };

                std::array<float, 3> const arr_v1 {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "v1")
                };
                std::array<float, 3> const arr_v2 {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "v2")
                };
                std::array<float, 3> const arr_v3 {
                    toml::find<std::array<float, 3>>(this->toml_obj, table_name, id, "v3")
                };

                vec::vec3_t const v1 {vec::vec3_t::from_array(arr_v1)};
                vec::vec3_t const v2 {vec::vec3_t::from_array(arr_v2)};
                vec::vec3_t const v3 {vec::vec3_t::from_array(arr_v3)};
                vec::vec3_t const normal {vec::vec3_t::surface_normal(v1, v2, v3)};

                prim::geo::triangle_t const triangle {v1, v2, v3, normal};

                lights.push_back(
                    std::make_unique<light::area_light_t>(
                        rgb::rgb_t<float>::from_array(power),
                        triangle
                    )
                );
            }
            else
                throw std::domain_error("Unknown type " + type + " for asset " + table_name);
        }

        return std::make_unique<scene::scene_t>(
            std::move(input_fn),
            std::move(lights)
        );
    }

    std::unique_ptr<shader::shader_t> build_shader() const override {

        std::unique_ptr<scene::scene_t> scene {this->build_scene()};

        if(!scene->is_loaded()){
            std::cerr << "Error loading scene!\n";
            std::exit(2);
        }
        else {
            std::cout << "scene loaded successfully\n";
            scene->print_summary();
            std::cout << '\n';
        }


        std::string const table_name {"shader"};
        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};

        if(type == "ambient"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            return std::make_unique<shader::ambient_shader_t>(
                std::move(scene),
                rgb::rgb_t<float>::from_array(bg)
            );
        }
        else if(type == "whitted"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            if(toml::find(this->toml_obj, table_name).contains("max_depth"))
                return std::make_unique<shader::whitted_shader_t>(
                    std::move(scene),
                    rgb::rgb_t<float>::from_array(bg),
                    toml::find<unsigned>(this->toml_obj, table_name, "max_depth")
                );
            else
                return std::make_unique<shader::whitted_shader_t>(
                    std::move(scene),
                    rgb::rgb_t<float>::from_array(bg)
                );
        }
        else if(type == "distributed"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            if(toml::find(this->toml_obj, table_name).contains("max_depth")){

                if(toml::find(this->toml_obj, table_name).contains("monte_carlo"))
                    return std::make_unique<shader::distributed_shader_t>(
                        std::move(scene),
                        rgb::rgb_t<float>::from_array(bg),
                        toml::find<unsigned>(this->toml_obj, table_name, "max_depth"),
                        toml::find<bool>(this->toml_obj, table_name, "monte_carlo")
                    );
                else
                    return std::make_unique<shader::distributed_shader_t>(
                        std::move(scene),
                        rgb::rgb_t<float>::from_array(bg),
                        toml::find<unsigned>(this->toml_obj, table_name, "max_depth")
                    );
            }
            else
                return std::make_unique<shader::distributed_shader_t>(
                    std::move(scene),
                    rgb::rgb_t<float>::from_array(bg)
                );
        }
        else
            throw std::domain_error("Unknown type " + type + " for asset " + table_name);
    }


    std::unique_ptr<render::renderer_t> build_renderer() const override {

        std::unique_ptr<cam::camera_t> cam {this->build_camera()};
        std::unique_ptr<shader::shader_t> shader {this->build_shader()};

        std::string const table_name {"renderer"};
        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};

        if(type == "standard"){

            if(toml::find(this->toml_obj, table_name).contains("spp"))
                return std::make_unique<render::standard_renderer_t>(
                    std::move(cam),
                    std::move(shader),
                    toml::find<unsigned>(this->toml_obj, table_name, "spp")
                );
            else
                return std::make_unique<render::standard_renderer_t>(
                    std::move(cam),
                    std::move(shader)
                );
        }
        else
            throw std::domain_error("Unknown type " + type + " for asset " + table_name);
    }

    std::unique_ptr<img::image_t> build_image() const override {

        std::unique_ptr<render::renderer_t> renderer {this->build_renderer()};

        std::string const table_name {"image"};

        std::string output_fn {
            toml::find<std::string>(this->toml_obj, table_name, "output_fn")
        };
        size_t const width {toml::find<size_t>(this->toml_obj, table_name, "width")};
        size_t const height {toml::find<size_t>(this->toml_obj, table_name, "height")};

        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};
        if(type == "ppm"){
            if(toml::find(this->toml_obj, table_name).contains("threads"))
                return std::make_unique<img::image_ppm_t>(
                    std::move(renderer), width, height, output_fn,
                    toml::find<unsigned>(this->toml_obj, table_name, "threads")
                );
            else
                return std::make_unique<img::image_ppm_t>(
                    std::move(renderer), width, height, output_fn
                );
        }
        else
            throw std::domain_error("Unknown type " + type + " for asset " + table_name);
    }

public:
    toml_config_parser_t(std::string const& fn) :
        toml_obj{toml::parse(fn)},
        use_degrees{toml::find_or(this->toml_obj, "use_degrees", true)} {}
};

std::unique_ptr<config_parser_t> config_parser_t::from_toml(std::string const& fn){
    return std::make_unique<toml_config_parser_t>(fn);
}

};
