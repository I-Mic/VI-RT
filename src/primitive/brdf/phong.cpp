#include "primitive/brdf/phong.hpp"
#include <tuple>

float phong_t::phong_normalization_term(float const shininess) const noexcept {
    return (1.f + shininess) * emath::INVERSE_TWO_PI;
}

rgb_t<float> phong_t::eval_specular(brdf_data_t const& data) const noexcept {
    rgb_t<float> const& specular_f0 {data.material->specular_f0};
    rgb_t<float> const& ks {data.material->ks};
    float const shininess {data.material->ns};

    vec3_t const& wi {data.wi.value()};
    vec3_t const& wo {data.wo.value()};
    vec3_t const& sn {data.sn.value()};

    if(shininess < 1000.f){

        vec3_t const rdir {vec3_t::reflect(wi, sn)};
        float const wi_dot_n {wi.dot(sn)};

        return specular_f0 * (
            std::pow(std::max(0.f, rdir.dot(wo)), shininess) *
            this->phong_normalization_term(shininess) *
            wi_dot_n
        );
    }
    else
        return ks;
}

std::tuple<vec3_t, float> phong_t::sample_specular(brdf_data_t const& data) const noexcept {

    vec3_t const& wo {data.wo.value()};
    vec3_t const& sn {data.sn.value()};
    float const shininess {data.material->ns};
    std::array<float, 2> const& rand_pair {data.rand_pair.value()};

    vec3_t const r {vec3_t::reflect(wo, sn)};

    if(shininess < 1000.f){
        float const cos_theta {std::pow(rand_pair[1], 1.f / (shininess + 1.f))};
        float const sin_theta {std::sqrt(1.f - cos_theta * cos_theta)};
        float const phi {emath::TWO_PI * rand_pair[0]};

        vec3_t const wi_phong {
            std::cos(phi) * sin_theta,
            std::sin(phi) * sin_theta,
            cos_theta
        };

        auto const& [rx, ry] {r.coordinate_system()};
        vec3_t const wi {wi_phong.rotate(rx, ry, r)};

        float const pdf {
            this->phong_normalization_term(shininess) *
            std::pow(cos_theta, shininess)
        };

        return std::make_tuple(wi, pdf);
    }
    else
        return std::make_tuple(r, 1.f);
}
