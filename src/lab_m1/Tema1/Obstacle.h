#pragma once
#include "utils/glm_utils.h"


class Obstacle {
public:
	Obstacle(glm::mat3 modelMatrix, glm::vec2 pos, glm::vec2 dim);
	~Obstacle();

	glm::mat3 GetModelMatrix();
	glm::vec2 GetPosition();
	glm::vec2 GetDimensions();

private:
	glm::mat3 modelMatrix;
	glm::vec2 position, dimensions;
};