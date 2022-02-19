#pragma once
#include "BPickup.h"

class DefenseBP :public BPickup {
public:
	DefenseBP();
	DefenseBP(glm::vec3 pos);
	~DefenseBP();
	void applyAction(float& health,Character& player);
};