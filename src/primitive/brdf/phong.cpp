#include "primitive/brdf/phong.hpp"

float phong_t::phong_normalization_term(float const shininess) const noexcept {
    return (1.f + shininess) * INVERSE_TWO_PI;
}

rgb_t<float> phong_t::eval_specular(brdf_data_t const& data) const noexcept {
    rgb_t<float> const& ks {data.material->ks};
    float const shininess {data.material->ns};

    vec3_t const& wi {data.wi.value()};
    vec3_t const& wo {data.wo.value()};
    vec3_t const& sn {data.sn.value()};

    vec3_t const rdir {vec3_t::reflect(wo, sn)};
    float const wi_dot_n {wi.dot(sn)};
    return ks *
        //this->phong_normalization_term(shininess) *
        std::pow(std::max(0.f, rdir.dot(wi)), shininess) *
        wi_dot_n;
}

std::tuple<vec3_t, float> phong_t::sample_specular(brdf_data_t const& data) const noexcept {

    vec3_t const& wo {data.wo.value()};
    float const shininess {data.material->ns};

    std::array<float, 2> const& rand_pair {data.rand_pair.value()};

    float const cos_theta {std::pow(rand_pair[1], 1.f / (shininess + 1.f))};
    float const sin_theta {std::sqrt(1.f - cos_theta * cos_theta)};
    float const phi {TWO_PI * rand_pair[0]};

    vec3_t const wi_phong {
        std::cos(phi) * sin_theta,
        std::sin(phi) * sin_theta,
        cos_theta
    };

    vec3_t const n_local {0.f, 0.f, 1.f};
    vec3_t const lobe_dir {vec3_t::reflect(wo, n_local)};
    auto const& [ldx, ldy] {lobe_dir.coordinate_system()};
    vec3_t const wi_local {wi_phong.rotate(ldx, ldy, lobe_dir)};

    float const pdf {
        //this->phong_normalization_term(shininess) *
        std::pow(cos_theta, shininess)
    };
    return std::make_tuple(wi_local, pdf);
}
