#include "Pickup.h"

Pickup::Pickup()
{
}

Pickup::Pickup(float width,float height,glm::vec3 color)
{
	radius = 15.f;
	mesh = myobject2D::CreateCircle("pickup", 1, color, true);
	pos = glm::vec2(randUtils::randInterval(radius, width - radius), randUtils::randInterval(radius, height - radius));
	
}

Pickup::~Pickup()
{
}

glm::vec2 Pickup::GetPos()
{
	return pos;
}

float Pickup::GetRadius()
{
	return radius;
}

glm::mat3 Pickup::GetModelMatrix()
{
	glm::mat3 m = glm::mat3(1);
	m *= transform2D::Translate(pos.x, pos.y);
	m *= transform2D::Scale(radius, radius);
	return m;
}

Mesh* Pickup::GetMesh()
{
	return mesh;
}

void Pickup::applyAction(HealthBar* hb,Player& player)
{
}
