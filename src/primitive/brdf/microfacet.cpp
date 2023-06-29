#include "primitive/brdf/microfacet.hpp"
#include "utils/math_extra.hpp"

#include <cmath>


// GGX distribution function
float microfacet_t::ndf(float const roughness, float const n_dot_h) const noexcept {
    float const alpha {roughness * roughness};
    float const alpha_sq {alpha * alpha};
    float const b {(alpha_sq - 1.f) * n_dot_h * n_dot_h + 1.f};
    return alpha_sq * emath::INVERSE_PI / (b * b);
}

float microfacet_t::geo_attenuation1(float const roughness, float n_dot_wx) const noexcept {
    /* optimized */
    float const alpha {roughness * roughness};
    float const alpha_sq {alpha * alpha};
    float const n_dot_wx_sq {n_dot_wx * n_dot_wx};
    return 2.f / (
        1.f + std::sqrt(
            1.f - alpha_sq + alpha_sq / n_dot_wx_sq
        )
    );
}

float microfacet_t::geo_attenuation2(
    float const roughness, float const n_dot_wi, float const n_dot_wo
) const noexcept {
    float const g1_wi {this->geo_attenuation1(roughness, n_dot_wi)};
    float const g1_wo {this->geo_attenuation1(roughness, n_dot_wo)};
    return g1_wo * g1_wi;
}

rgb_t<float> microfacet_t::fresnel_schlick(
    rgb_t<float> const& f0, float const h_dot_wx, rgb_t<float> const& f90
) const noexcept {
    return f0 + (f90 - f0) * std::pow(1.f - h_dot_wx, 5.f);
}

rgb_t<float> microfacet_t::eval_specular(brdf_data_t const& data) const noexcept {

    float const roughness {data.material->roughness};
    rgb_t<float> const& f0 {data.material->specular_f0};
    vec3_t const& sn {data.sn.value()};
    vec3_t const& wi {data.wi.value()};
    vec3_t const& wo {data.wo.value()};
    vec3_t const& half {data.half.value()};

    float const n_dot_wo {emath::clamp(sn.dot(wo), 0.00001f, 1.f)};
    float const n_dot_wi {emath::clamp(sn.dot(wi), 0.00001f, 1.f)};
    float const n_dot_h  {emath::clamp(sn.dot(half), 0.00001f, 1.f)};
    float const h_dot_wo {emath::clamp(wo.dot(half), 0.00001f, 1.f)};

    float const d {this->ndf(roughness, n_dot_h)};
    float const g2 {this->geo_attenuation2(roughness, n_dot_wi, n_dot_wo)};
    rgb_t<float> const f {this->fresnel_schlick(f0, h_dot_wo)};

    return f * (g2 * d / (4.f * n_dot_wi * n_dot_wo));
}

std::tuple<vec3_t, float> microfacet_t::sample_specular(brdf_data_t const& data) const noexcept {

    float const roughness {data.material->roughness};
    float const alpha {roughness * roughness};
    vec3_t const& wo {data.wo.value()};
    vec3_t const& sn {data.sn.value()};

    vec4_t const rz_quaternion {vec4_t::get_rotation_to_z_axis(sn)};
    vec3_t const wo_local {rz_quaternion.rotate(wo)};
    vec3_t const n_local {0.f, 0.f, 1.f};
    vec3_t h_local {};

    if(roughness == 0.f)
        h_local = {0.f, 0.f, 1.f};
    else {

        /* Samples a microfacet normal for the GGX distribution using VNDF method.
        * Source: "Sampling the GGX Distribution of Visible Normals" by Heitz
        * See also https://hal.inria.fr/hal-00996995v1/document
        * and http://jcgt.org/published/0007/04/01/
        * PDF is 'G1(NdotV) * D'
        */

        // Section 3.2: transforming the view direction to the hemisphere configuration
        vec3_t const wo_h {
            vec3_t::normalized({alpha * wo_local.x, alpha * wo_local.y, wo_local.z})
        };

        // Section 4.1: orthonormal basis (with special case if cross product is zero)
        float const lensq {wo_h.x * wo_h.x + wo_h.y * wo_h.y};
        vec3_t const t1 {
            lensq > 0.f ?
                vec3_t{-wo_h.y, wo_h.x, 0.f} / std::sqrt(lensq) :
                vec3_t{1.f, 0.f, 0.f}
        };
        vec3_t const t2 {wo_h.cross(t1)};

        // Section 4.2: parameterization of the projected area
        std::array<float, 2> const rand_pair {emath::rand_tuple<2>()};

        float const r {std::sqrt(rand_pair[0])};
        float const phi {emath::TWO_PI * rand_pair[1]};
        float const u1 {r * std::cos(phi)};
        float const u2 {r * std::sin(phi)};
        float const s {0.5f * (1.f + wo_h.z)};
        float const u3 {std::sqrt(1.f - u1 * u1) * (1.f - s) + u2 * s};

        // Section 4.3: reprojection onto hemisphere
        vec3_t const n_h {
            u1 * t1 +
            u3 * t2 +
            std::sqrt(
                std::max(0.f, 1.f - u1 * u1 - u3 * u3)
            ) * wo_h
        };

        // Section 3.4: transforming the normal back to the ellipsoid configuration
        h_local = vec3_t::normalized({alpha * n_h.x, alpha * n_h.y, std::max(0.f, n_h.z)});
    }

    vec3_t const wi_local {vec3_t::reflect(wo_local, h_local)};

    float const n_dot_wo {emath::clamp(n_local.dot(wo_local), 0.00001f, 1.f)};
    float const n_dot_h  {emath::clamp(n_local.dot(h_local),  0.00001f, 1.f)};

    float const pdf {
        this->ndf(roughness, n_dot_h) *
        this->geo_attenuation1(roughness, n_dot_wo)
    };

    // convert from local space
    vec3_t const wi {vec3_t::normalized(rz_quaternion.invert_rotation().rotate(wi_local))};

    return std::make_tuple(wi, pdf);
}
