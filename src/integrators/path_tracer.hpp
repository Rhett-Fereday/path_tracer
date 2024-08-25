#pragma once

#include <image_buffers/hdr_buffer.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <hit_info.hpp>
#include <glm/glm.hpp>
#include <samplers/random_sampler.hpp>

namespace pt::integrators
{
    class path_tracer
    {
    public:
        using buffer_type = image_buffers::hdr_buffer;

        struct evaluation_properties
        {
            unsigned int bounce_limit{2u};
        };

        [[nodiscard]] glm::vec3 evaluate(scene const& s, ray r, evaluation_properties const& properties) const noexcept
        {
            // auto const hit_optional = s.closest_hit(r);

            // if(!hit_optional) return glm::vec3{0.0f};

            // auto const& shape = s.shapes[hit_optional->shape_id];
            // auto const& mat = s.materials[std::get<shapes::sphere>(shape).material_ID];

            // return mat.albedo;
            
            glm::vec3 l{0.0f}, beta{1.0f};
            pt::samplers::random_sampler sampler;
            constexpr float epsilon = 0.001f;

            for (int bounce = 0; bounce < properties.bounce_limit; ++bounce)
            {
                auto const hit_optional = s.closest_hit(r);

                if (!hit_optional)
                {
                    l += beta * glm::vec3{0.0f};
                    break;
                }

                auto const& shape = s.shapes[hit_optional->shape_id];
                auto const& mat = s.materials[std::get<shapes::sphere>(shape).material_ID];

                l += beta * mat.emissiveness;

                // Sample new direction assuming all objects are Lambertian
                // Calculate orthonormal basis from the normal
                glm::vec3 const& w = hit_optional->normal;
                glm::vec3 const u = glm::normalize(glm::vec3(w.z - w.y, w.x - w.z, w.y - w.x));
                glm::vec3 const v = glm::normalize(glm::cross(w, u));

                // Generate two random floats in range [0,1]
                float const rand1 = sampler.generate_1D();
                float const rand2 = sampler.generate_1D();

                float const root = glm::sqrt(rand2);
                float const angle = 2.0f * glm::pi<float>() * rand1;

                // Calculate the x, y, and z components of a new random ray assuming a lambertian BRDF
                float const a = glm::cos(angle) * root;
                float const b = glm::sin(angle) * root;
                float const c = glm::sqrt(1.0f - rand2);

                r = { hit_optional->position + w * epsilon,
                                 glm::normalize(a * u + b * v + c * w) };

                // Lambertian Rho = R/pi
                auto const f = mat.albedo / glm::pi<float>();
                auto const cos_wi = glm::max(glm::dot(w, r.direction), 0.0f);//glm::abs(glm::dot(interaction->geometricNormal, Wi));
                float const pdf = cos_wi / glm::pi<float>();
                // float const inv_pdf = glm::pi<float>() / cos_wi;

                if(pdf == 0.0f) break;
                // if (inv_pdf == 0.0f) break;

                // Update Beta
                beta *= f * cos_wi / pdf;
                // beta *= f * cos_wi * inv_pdf;
                //Beta *= (glm::max(glm::dot(interaction->geometricNormal, Wi), 0.0f));
            }

            return l;
        }
    };
}