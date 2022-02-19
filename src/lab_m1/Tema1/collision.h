#pragma once
#include "utils/glm_utils.h"


namespace collision {
	bool circleRectCollision(glm::vec2 circCenter, float radius, glm::vec2 pos, glm::vec2 dim);
	bool circleWithinBoundary(glm::vec2 pos, float radius, float width, float height);
	bool circleCircleCollision(glm::vec2 center1, float r1, glm::vec2 center2, float r2);
	bool rectRectCollision(glm::vec2 pos, glm::vec2 dim, glm::vec2 pos2, glm::vec2 dim2);
	bool rectWithinBoundary(glm::vec2 pos, glm::vec2 dim, float width, float height);
}
