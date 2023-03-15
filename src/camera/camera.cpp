#include "camera/camera.hpp"

namespace cam {

camera_t::camera_t(size_t const width, size_t const height) noexcept :
    width{width}, height{height} {}

camera_t::~camera_t(){}

};
