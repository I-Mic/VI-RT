#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <memory>
#include <string>

#include "camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene.hpp"
#include "shader/shader.hpp"
#include "image/image.hpp"

namespace config {

class config_parser_t {

private:

	config_parser_t(config_parser_t const&) = delete;
	config_parser_t(config_parser_t&&) = delete;
	config_parser_t& operator=(config_parser_t const&) = delete;
	config_parser_t& operator=(config_parser_t&&) = delete;

	virtual std::unique_ptr<cam::camera_t> build_camera() const = 0;
	virtual std::unique_ptr<scene::scene_t> build_scene() const = 0;
	virtual std::unique_ptr<shader::shader_t> build_shader() const = 0;
	virtual std::unique_ptr<render::renderer_t> build_renderer() const = 0;

protected:

	config_parser_t() noexcept;

public:

	virtual ~config_parser_t();

	static std::unique_ptr<config_parser_t> from_toml(std::string const& fn);
	// other factories for other types of configurations

	virtual std::unique_ptr<img::image_t> build_image() const = 0;
};

};

#endif //CONFIG_PARSER_HPP
