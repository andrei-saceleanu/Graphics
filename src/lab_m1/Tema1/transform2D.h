#pragma once

#include "utils/glm_utils.h"
#define PI 3.14159265

namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(
            glm::mat3(1.f, 0.f, translateX,
            0.f, 1.f, translateY,
            0.f, 0.f, 1));

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(glm::mat3(
            scaleX, 0.f, 0.f,
            0.f, scaleY, 0.f,
            0.f, 0.f, 1.f
        ));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        float c = cos(radians), s = sin(radians);
        return glm::transpose(glm::mat3(
            c, -s, 0.f,
            s, c, 0.f,
            0.f, 0.f, 1.f
        ));

    }
}   // namespace transform2D
