#ifndef PHONG_HPP
#define PHONG_HPP

#include "primitive/brdf/brdf.hpp"

// Phong reflection model includes Lambert's diffuse BRDF

class lambertian_t : public diffuse_brdf_t {

public:
    lambertian_t() noexcept  = default;
    ~lambertian_t() noexcept = default;

    rgb_t<float> eval_diffuse(brdf_data_t const& data) const noexcept override;
    float diffuse_term(brdf_data_t const&) const noexcept override;
};

class phong_t : public specular_brdf_t {

private:
    float phong_normalization_term(float const shininess) const noexcept;

public:

    phong_t() noexcept  = default;
    ~phong_t() noexcept = default;

    virtual rgb_t<float> eval_specular(brdf_data_t const& data) const noexcept override;
    //virtual float pdf(brdf_data_t const& data) const noexcept override;
    //virtual std::tuple<vec3_t, float> sample_specular_half_vec(
    //    brdf_data_t const& data
    //) const noexcept override;
    virtual std::tuple<vec3_t, float> sample_specular(
        brdf_data_t const& data
    ) const noexcept override;
};

#endif /* PHONG_HPP */
