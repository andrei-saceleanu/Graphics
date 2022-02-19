#include "collision.h"

bool collision::circleRectCollision(glm::vec2 circCenter, float radius, glm::vec2 pos, glm::vec2 dim)
{
	glm::vec2 centerRect(pos.x + dim.x / 2, pos.y + dim.y / 2);
	float diagLength = glm::length(dim);
	glm::vec2 v = circCenter - centerRect;
	if (glm::length(v) >= radius + diagLength) {
		return false;
	}
	glm::vec2 halfDim(dim.x / 2, dim.y / 2);
	glm::vec2 clamped = glm::clamp(v, -halfDim, halfDim);
	glm::vec2 closest = centerRect + clamped;
	v = circCenter - closest;
	return glm::length(v) < radius;
}


bool collision::circleWithinBoundary(glm::vec2 pos, float radius, float width, float height)
{
	return (pos.x - radius > 0) && (pos.x + radius < width) && (pos.y - radius > 0) && (pos.y + radius < height);
}

bool collision::circleCircleCollision(glm::vec2 center1, float r1, glm::vec2 center2, float r2)
{
	float d = glm::length(center1-center2);
	return d <= r1 + r2;
}

bool collision::rectRectCollision(glm::vec2 pos, glm::vec2 dim, glm::vec2 pos2, glm::vec2 dim2)
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

bool collision::rectWithinBoundary(glm::vec2 pos, glm::vec2 dim, float width, float height)
{
	if (pos.x < 0 || pos.y<0 || pos.x + dim.x>width || pos.y + dim.y>height) {
		return false;
	}
	return true;
}
