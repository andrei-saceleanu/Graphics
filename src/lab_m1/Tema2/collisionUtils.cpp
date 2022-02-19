#include "collisionUtils.h"


bool collisionUtils::sphereBoxCollision(glm::vec3 circCenter, float radius, glm::vec3 pos, glm::vec3 dim)
{
	glm::vec3 centerRect(pos.x + dim.x / 2, pos.y + dim.y / 2, pos.z + dim.z / 2);
	float diagLength = glm::length(dim);
	glm::vec3 v = circCenter - centerRect;
	if (glm::length(v) >= radius + 0.5f * diagLength) {
		return false;
	}
	glm::vec3 halfDim(dim.x / 2, dim.y / 2, dim.z / 2);
	glm::vec3 clamped = glm::clamp(v, -halfDim, halfDim);
	glm::vec3 closest = centerRect + clamped;
	v = circCenter - closest;
	return glm::length(v) < radius;
}

bool collisionUtils::rectRectCollision(glm::vec2 pos, glm::vec2 dim, glm::vec2 pos2, glm::vec2 dim2)
{
	if (pos2.x > pos.x + dim.x) {
		return false;
	}
	if (pos2.x + dim2.x < pos.x) {
		return false;
	}
	if (pos2.y > pos.y + dim.y) {
		return false;
	}
	if (pos2.y + dim2.y < pos.y) {
		return false;
	}
	return true;
}

bool collisionUtils::sphereSphereCollision(glm::vec3 circCenter, float radius, glm::vec3 circCenter2, float radius2)
{
	return glm::length(circCenter - circCenter2) <= radius + radius2;
}
