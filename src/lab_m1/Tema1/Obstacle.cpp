#include "Obstacle.h"

Obstacle::Obstacle(glm::mat3 modelMatrix, glm::vec2 pos, glm::vec2 dim)
{
	this->modelMatrix = modelMatrix;
	this->position = pos;
	this->dimensions = dim;
}

Obstacle::~Obstacle()
{
}

glm::mat3 Obstacle::GetModelMatrix()
{
	return modelMatrix;
}

glm::vec2 Obstacle::GetPosition()
{
	return position;
}

glm::vec2 Obstacle::GetDimensions()
{
	return dimensions;
}
