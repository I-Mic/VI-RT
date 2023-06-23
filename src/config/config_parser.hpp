#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <memory>
#include <string>

#include "camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "image/image.hpp"

class config_parser_t {

private:

    virtual std::unique_ptr<camera_t> build_camera() const     = 0;
    virtual std::unique_ptr<scene_t> build_scene() const       = 0;
    virtual std::unique_ptr<shader_t> build_shader() const     = 0;
    virtual std::unique_ptr<renderer_t> build_renderer() const = 0;

protected:

    config_parser_t() noexcept = default;

public:

    virtual ~config_parser_t() noexcept = default;

    config_parser_t(config_parser_t const&) = delete;
    config_parser_t(config_parser_t&&)      = delete;
    config_parser_t& operator=(config_parser_t const&) = delete;
    config_parser_t& operator=(config_parser_t&&)      = delete;

    static std::unique_ptr<config_parser_t> from_toml(std::string const& fn);
    // other factories for other types of configurations

    virtual std::unique_ptr<image_t> build_image() const = 0;
};

#endif //CONFIG_PARSER_HPP
