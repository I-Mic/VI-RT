#ifndef LAMBERT_HPP
#define LAMBERT_HPP

#include "primitive/brdf/brdf.hpp"

// Phong reflection model includes Lambert's diffuse BRDF

class lambert_t : public diffuse_brdf_t {

public:
    lambert_t() noexcept  = default;
    ~lambert_t() noexcept = default;

    rgb_t<float> eval_diffuse(brdf_data_t const& data) const noexcept override;
    float diffuse_term(brdf_data_t const&) const noexcept override;
};

#endif /* LAMBERT_HPP */
