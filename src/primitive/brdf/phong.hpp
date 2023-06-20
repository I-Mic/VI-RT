#ifndef PHONG_HPP
#define PHONG_HPP

#include "primitive/brdf/brdf.hpp"

class phong_t : public specular_brdf_t {

private:
    float phong_normalization_term(float const shininess) const noexcept;

public:

    phong_t() noexcept  = default;
    ~phong_t() noexcept = default;

    rgb_t<float> eval_specular(brdf_data_t const& data) const noexcept override;
    //virtual float pdf(brdf_data_t const& data) const noexcept override;
    //virtual std::tuple<vec3_t, float> sample_specular_half_vec(
    //    brdf_data_t const& data
    //) const noexcept override;
    std::tuple<vec3_t, float> sample_specular(brdf_data_t const& data) const noexcept override;
};

#endif /* PHONG_HPP */
