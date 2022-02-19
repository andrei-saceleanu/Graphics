#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(glm::mat4(
            1.f, 0.f, 0.f, translateX,
            0.f, 1.f, 0.f, translateY,
            0.f, 0.f, 1.f, translateZ,
            0.f, 0.f, 0.f, 1.f
        ));

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Implement the scaling matrix
        return glm::mat4(
            scaleX, 0.f, 0.f, 0.f,
            0.f, scaleY, 0.f, 0.f,
            0.f, 0.f, scaleZ, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        float c = cos(radians), s = sin(radians);
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(
            c, s, 0.f, 0.f,
            -s, c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        ));

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        float c = cos(radians), s = sin(radians);
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(
            c, 0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
            s, 0.f, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        ));

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        float c = cos(radians), s = sin(radians);
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, c, s, 0.f,
            0.f, -s, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        ));

    }
}   // namespace transform3D