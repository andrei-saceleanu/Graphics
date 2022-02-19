#pragma once
#include "utils/glm_utils.h"


namespace collisionUtils {
	bool sphereBoxCollision(glm::vec3 circCenter, float radius, glm::vec3 pos, glm::vec3 dim);
	bool rectRectCollision(glm::vec2 pos, glm::vec2 dim, glm::vec2 pos2, glm::vec2 dim2);
	bool sphereSphereCollision(glm::vec3 circCenter, float radius, glm::vec3 circCenter2, float radius2);

}