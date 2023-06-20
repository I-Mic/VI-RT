#include "primitive/brdf/microfacet.hpp"

#include <cmath>
#include <cstdlib>


float microfacet_t::ndf(float const roughness, float const n_dot_h) const noexcept {
    float const roughness_squared {roughness * roughness};
    float const b {(roughness_squared - 1.f) * n_dot_h * n_dot_h + 1.f};
    return roughness_squared * INVERSE_PI / (b * b);
}

float microfacet_t::geo_attenuation1(float const roughness, float h_dot_wx) const noexcept {
    /*float const a {
        h_dot_wx / (
            std::max(0.00001f, roughness) *
            std::sqrt(1 - std::min(0.99999f, h_dot_wx * h_dot_wx))
        )
    };
    float const lambda {-0.5f + std::sqrt(1.f + 1.f / (a * a)) * 0.5f};
    return 1.f / (1.f + lambda);*/

    /* optimized */
    float const roughness_squared {roughness * roughness};
    float const h_dot_wx_squared {h_dot_wx * h_dot_wx};
    return 2.f / (
        1.f + std::sqrt(
            1.f - roughness_squared + roughness_squared / h_dot_wx_squared
        )
    );
}

float microfacet_t::geo_attenuation2(
    float const roughness,
    vec3_t const& wi,
    vec3_t const& wo,
    vec3_t const& h
) const noexcept {
    float const g1_wo {this->geo_attenuation1(roughness, h.dot(wo))};
    float const g1_wi {this->geo_attenuation1(roughness, h.dot(wi))};
    return 1.f / (1.f + g1_wo + g1_wi);
}

rgb_t<float> microfacet_t::fresnel_schlick(
    rgb_t<float> const& f0,
    float const h_dot_wx
) const noexcept {
    static rgb_t<float> const f90 {1.f, 1.f, 1.f};
    return f0 + (f90 - f0) * std::pow(1.f - h_dot_wx, 5.f);
}

rgb_t<float> microfacet_t::eval_specular(brdf_data_t const& data) const noexcept {

    float const roughness {data.material->roughness};
    rgb_t<float> const& f0 {data.material->f0};
    vec3_t const& sn {data.sn.value()};
    vec3_t const& wi {data.wi.value()};
    vec3_t const& wo {data.wo.value()};
    vec3_t const& half {data.half.value()};

    float const n_dot_h {sn.dot(half)};
    float const n_dot_wi {sn.dot(wi)};
    float const n_dot_wo {sn.dot(wo)};
    float const h_dot_wo {half.dot(wo)};

    float const d {this->ndf(roughness, n_dot_h)};
    float const g2 {this->geo_attenuation2(roughness, wi, wo, half)};
    rgb_t<float> const f {this->fresnel_schlick(f0, h_dot_wo)};

    return f * (g2 * d / (4.f * n_dot_wi * n_dot_wo));
}

std::tuple<vec3_t, float> microfacet_t::sample_specular(brdf_data_t const& data) const noexcept {

    float const roughness {data.material->roughness};
    //float const roughness_squared {roughness * roughness};
    vec3_t const& wo {data.wo.value()};

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
        vec3_t const wo_h {vec3_t::normalize({roughness * wo.x, roughness * wo.y, wo.z})};

        // Section 4.1: orthonormal basis (with special case if cross product is zero)
        float const lensq {wo_h.x * wo_h.x + wo_h.y * wo_h.y};
        vec3_t const t1 {
            lensq > 0.f
                ? vec3_t{-wo_h.y, wo_h.x, 0.f} / std::sqrt(lensq)
                : vec3_t{1.f, 0.f, 0.f}
        };
        vec3_t const t2 {wo_h.cross(t1)};

        // Section 4.2: parameterization of the projected area
        std::array<float, 2> const rand_pair {
            std::rand() / static_cast<float>(RAND_MAX),
            std::rand() / static_cast<float>(RAND_MAX)
        };

        float const r {std::sqrt(rand_pair[0])};
        float const phi {TWO_PI * rand_pair[1]};
        float const u1 {r * std::cos(phi)};
        float u2 {r * std::sin(phi)};
        float const s {0.5f * (1.f + wo_h.z)};
        u2 = std::sqrt(1.f - u1 * u1) * (1.f - s) + u2 * s;

        // Section 4.3: reprojection onto hemisphere
        vec3_t const nh {
            u1 * t1 +
            u2 * t2 +
            std::sqrt(
                std::max(0.f, 1.f - u1 * u1 - u2 * u2)
            ) * wo_h
        };

        // Section 3.4: transforming the normal back to the ellipsoid configuration
        h_local = vec3_t::normalize({roughness * nh.x, roughness * nh.y, std::max(0.f, nh.z)});
    }

    vec3_t const wi_local {vec3_t::reflect(wo, h_local)};

    vec3_t const n_local {0.f, 0.f, 1.f};
    float const n_dot_wo {std::max(0.00001f, std::min(1.f, n_local.dot(wo)))};
    float const n_dot_h  {std::max(0.00001f, std::min(1.f, n_local.dot(h_local)))};


    //float3 F = evalFresnel(specularF0, shadowedF90(specularF0), HdotL);

    // Calculate weight of the sample specific for selected sampling method
    // (this is microfacet BRDF divided by PDF of sampling method - notice how most terms cancel out)
    //weight = F * specularSampleWeight(alpha, alphaSquared, NdotL, NdotV, HdotL, NdotH);

    float const pdf {
        this->ndf(roughness, n_dot_h) *
        this->geo_attenuation1(roughness, n_dot_wo) /
        (4.f * n_dot_wo)
    };

    return std::make_tuple(wi_local, pdf);
}
