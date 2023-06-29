#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>

#include "camera/perspective.hpp"
#include "config/config_parser.hpp"
#include "image/image_ppm.hpp"
#include "light/ambient_light.hpp"
#include "light/area_light.hpp"
#include "light/point_light.hpp"
#include "primitive/geometry/triangle.hpp"
#include "primitive/brdf/lambert.hpp"
#include "primitive/brdf/phong.hpp"
#include "primitive/brdf/microfacet.hpp"
#include "renderer/renderer.hpp"
#include "renderer/standard_renderer.hpp"
#include "scene/scene.hpp"
#include "shader/ambient_shader.hpp"
#include "shader/distributed_shader.hpp"
#include "shader/path_tracer_shader.hpp"
#include "shader/whitted_shader.hpp"
#include "toml.hpp"
#include "utils/math_extra.hpp"

class toml_config_parser_t : public config_parser_t {

private:

    toml::value const toml_obj;
    bool const use_degrees;

    std::unique_ptr<camera_t> build_camera() const override {

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

            return std::make_unique<perspective_t>(
                vec3_t::from_array(eye),
                vec3_t::from_array(at),
                vec3_t::from_array(up),
                width, height,
                this->use_degrees ? emath::degrees_to_rads(fov_w) : fov_w,
                this->use_degrees ? emath::degrees_to_rads(fov_h) : fov_h
            );
        }
        else
            throw std::domain_error("Unknown type '" + type + "' for asset '" + table_name + "'");
    }


    std::unique_ptr<scene_t> build_scene() const override {

        std::string const table_name {"scene"};

        std::string const input_fn {
            toml::find<std::string>(this->toml_obj, table_name, "input_fn")
        };


        std::vector<std::unique_ptr<light_t>> lights {};
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
                    std::make_unique<ambient_light_t>(
                        rgb_t<float>::from_array(color)
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
                    std::make_unique<point_light_t>(
                        rgb_t<float>::from_array(color),
                        vec3_t::from_array(pos)
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

                vec3_t const v1 {vec3_t::from_array(arr_v1)};
                vec3_t const v2 {vec3_t::from_array(arr_v2)};
                vec3_t const v3 {vec3_t::from_array(arr_v3)};
                vec3_t const normal {vec3_t::surface_normal(v1, v2, v3)};

                triangle_t const triangle {v1, v2, v3, normal};

                lights.push_back(
                    std::make_unique<area_light_t>(
                        rgb_t<float>::from_array(power),
                        triangle
                    )
                );
            }
            else
            throw std::domain_error("Unknown type '" + type + "' for asset '" + table_name + "'");
        }

        return std::make_unique<scene_t>(
            std::move(input_fn),
            std::move(lights)
        );
    }

    std::unique_ptr<shader_t> build_shader() const override {

        std::unique_ptr<scene_t> scene {this->build_scene()};

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

            return std::make_unique<ambient_shader_t>(
                std::move(scene),
                rgb_t<float>::from_array(bg)
            );
        }
        else if(type == "whitted"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            std::string const specular_brdf_name {
                toml::find<std::string>(this->toml_obj, table_name, "brdf")
            };

            std::unique_ptr<specular_brdf_t> specular_brdf {nullptr};
            if(specular_brdf_name == "phong")
                specular_brdf.reset(new phong_t{});
            else if(specular_brdf_name == "microfacet")
                specular_brdf.reset(new microfacet_t{});
            else
                throw std::domain_error("Unknown brdf name '" + specular_brdf_name + "'");

            if(toml::find(this->toml_obj, table_name).contains("max_depth"))
                return std::make_unique<whitted_shader_t>(
                    std::move(scene),
                    std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                    std::move(specular_brdf),
                    rgb_t<float>::from_array(bg),
                    toml::find<unsigned>(this->toml_obj, table_name, "max_depth")
                );
            else
                return std::make_unique<whitted_shader_t>(
                    std::move(scene),
                    std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                    std::move(specular_brdf),
                    rgb_t<float>::from_array(bg)
                );
        }
        else if(type == "distributed"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            std::string const specular_brdf_name {
                toml::find<std::string>(this->toml_obj, table_name, "brdf")
            };

            std::unique_ptr<specular_brdf_t> specular_brdf {nullptr};
            if(specular_brdf_name == "phong")
                specular_brdf.reset(new phong_t{});
            else if(specular_brdf_name == "microfacet")
                specular_brdf.reset(new microfacet_t{});
            else
                throw std::domain_error("Unknown brdf name '" + specular_brdf_name + "'");

            if(toml::find(this->toml_obj, table_name).contains("max_depth"))
                return std::make_unique<distributed_shader_t>(
                    std::move(scene),
                    std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                    std::move(specular_brdf),
                    rgb_t<float>::from_array(bg),
                    toml::find<unsigned>(this->toml_obj, table_name, "max_depth")
                );
            else
                return std::make_unique<distributed_shader_t>(
                    std::move(scene),
                    std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                    std::move(specular_brdf),
                    rgb_t<float>::from_array(bg)
                );
        }
        else if(type == "path_tracer"){

            std::array<float, 3> const bg {
                toml::find<std::array<float, 3>>(this->toml_obj, table_name, "bg")
            };

            std::string const specular_brdf_name {
                toml::find<std::string>(this->toml_obj, table_name, "brdf")
            };

            std::unique_ptr<specular_brdf_t> specular_brdf {nullptr};
            if(specular_brdf_name == "phong")
                specular_brdf.reset(new phong_t{});
            else if(specular_brdf_name == "microfacet")
                specular_brdf.reset(new microfacet_t{});
            else
                throw std::domain_error("Unknown brdf name '" + specular_brdf_name + "'");

            if(toml::find(this->toml_obj, table_name).contains("max_depth")){

                if(toml::find(this->toml_obj, table_name).contains("p_continue"))
                    return std::make_unique<path_tracer_shader_t>(
                        std::move(scene),
                        std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                        std::move(specular_brdf),
                        rgb_t<float>::from_array(bg),
                        toml::find<unsigned>(this->toml_obj, table_name, "max_depth"),
                        toml::find<float>(this->toml_obj, table_name, "p_continue")
                    );
                else
                    return std::make_unique<path_tracer_shader_t>(
                        std::move(scene),
                        std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                        std::move(specular_brdf),
                        rgb_t<float>::from_array(bg),
                        toml::find<unsigned>(this->toml_obj, table_name, "max_depth")
                    );
            }
            else
                return std::make_unique<path_tracer_shader_t>(
                    std::move(scene),
                    std::unique_ptr<diffuse_brdf_t>{std::make_unique<lambert_t>()},
                    std::move(specular_brdf),
                    rgb_t<float>::from_array(bg)
                );
        }
        else
            throw std::domain_error("Unknown type '" + type + "' for asset '" + table_name + "'");
    }


    std::unique_ptr<renderer_t> build_renderer() const override {

        std::unique_ptr<camera_t> cam {this->build_camera()};
        std::unique_ptr<shader_t> shader {this->build_shader()};

        std::string const table_name {"renderer"};
        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};

        if(type == "standard"){

            if(toml::find(this->toml_obj, table_name).contains("spp"))
                return std::make_unique<standard_renderer_t>(
                    std::move(cam),
                    std::move(shader),
                    toml::find<unsigned>(this->toml_obj, table_name, "spp")
                );
            else
                return std::make_unique<standard_renderer_t>(
                    std::move(cam),
                    std::move(shader)
                );
        }
        else
            throw std::domain_error("Unknown type '" + type + "' for asset '" + table_name + "'");
    }

    std::unique_ptr<image_t> build_image() const override {

        std::unique_ptr<renderer_t> renderer {this->build_renderer()};

        std::string const table_name {"image"};

        std::string output_fn {toml::find<std::string>(this->toml_obj, table_name, "output_fn")};
        size_t const width {toml::find<size_t>(this->toml_obj, table_name, "width")};
        size_t const height {toml::find<size_t>(this->toml_obj, table_name, "height")};

        std::string const type {toml::find<std::string>(this->toml_obj, table_name, "type")};
        if(type == "ppm"){
            if(toml::find(this->toml_obj, table_name).contains("threads")){
                if(toml::find(this->toml_obj, table_name).contains("normalize"))
                    return std::make_unique<image_ppm_t>(
                        std::move(renderer), width, height, output_fn,
                        toml::find<unsigned>(this->toml_obj, table_name, "threads"),
                        toml::find<bool>(this->toml_obj, table_name, "normalize")
                    );
                else
                    return std::make_unique<image_ppm_t>(
                        std::move(renderer), width, height, output_fn,
                        toml::find<unsigned>(this->toml_obj, table_name, "threads")
                    );
            }
            else
                return std::make_unique<image_ppm_t>(
                    std::move(renderer), width, height, output_fn
                );
        }
        else
            throw std::domain_error("Unknown type '" + type + "' for asset '" + table_name + "'");
    }

public:
    toml_config_parser_t(std::string const& fn) :
        toml_obj{toml::parse(fn)},
        use_degrees{toml::find_or(this->toml_obj, "use_degrees", true)} {}
};

std::unique_ptr<config_parser_t> config_parser_t::from_toml(std::string const& fn){
    return std::make_unique<toml_config_parser_t>(fn);
}
