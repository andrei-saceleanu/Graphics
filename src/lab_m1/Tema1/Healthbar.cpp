#include "Healthbar.h"

HealthBar::HealthBar()
{
}

HealthBar::HealthBar(float offsetX, float offsetY)
{
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->scale = 100;
    frame = myobject2D::CreateSquare("frame", glm::vec3(0, 0, 0), 1, glm::vec3(0, 1, 0));
    body = myobject2D::CreateSquare("body", glm::vec3(0, 0, 0), 1, glm::vec3(0, 1, 0),true);
}

HealthBar::~HealthBar()
{
}

Mesh* HealthBar::GetFrameMesh()
{
    return frame;
}

Mesh* HealthBar::GetBodyMesh()
{
    return body;
}

glm::mat3 HealthBar::GetFrameMatrix()
{
    glm::mat3 frameMatrix = glm::mat3(1);
    frameMatrix *= transform2D::Translate(offsetX, offsetY);
    frameMatrix *= transform2D::Scale(1.75f*100, 50);

    return frameMatrix;
}

glm::mat3 HealthBar::GetBodyMatrix()
{
    glm::mat3 bodyMatrix = glm::mat3(1);
    bodyMatrix *= transform2D::Translate(offsetX, offsetY);
    bodyMatrix *= transform2D::Scale(1.75f*this->scale, 50);

    return bodyMatrix;
}

void HealthBar::changeOffsetX(float delta)
{
    offsetX += delta;
}

void HealthBar::changeOffsetY(float delta)
{
    offsetY += delta;
}

void HealthBar::changeScale(float delta)
{
    scale = (scale + delta<=100)?(scale+delta):100;
}

float HealthBar::GetScale()
{
    return scale;
}
