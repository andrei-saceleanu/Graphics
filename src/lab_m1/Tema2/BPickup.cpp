#include "BPickup.h"

BPickup::BPickup()
{
}

BPickup::BPickup(glm::vec3 pos)
{
	this->pos = pos;
}

BPickup::~BPickup()
{
}

glm::vec3 BPickup::getColor()
{
	return color;
}

glm::vec3 BPickup::getPos()
{
	return pos;
}

glm::mat4 BPickup::getModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	return modelMatrix;
}

void BPickup::applyAction(float& health,Character& player)
{
}
