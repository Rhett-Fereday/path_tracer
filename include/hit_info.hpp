#pragma once

#include <glm/glm.hpp>

namespace pt
{
    struct hit_info
    {
        glm::vec3 position, normal;
        unsigned int shape_id;
        float distance;
    };
}