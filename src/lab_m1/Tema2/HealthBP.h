#pragma once
#include "BPickup.h"

class HealthBP :public BPickup {
public:
	HealthBP();
	HealthBP(glm::vec3 pos);
	~HealthBP();
	void applyAction(float& health,Character& player);
};