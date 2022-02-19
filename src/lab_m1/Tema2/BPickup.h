#pragma once
#include "utils/glm_utils.h"
#include "Character.h"


class BPickup {
public:
	BPickup();
	BPickup(glm::vec3 pos);
	~BPickup();
	glm::vec3 getColor();
	glm::vec3 getPos();
	glm::mat4 getModelMatrix();
	virtual void applyAction(float& health,Character& player);

protected:
	glm::vec3 color, pos;
};