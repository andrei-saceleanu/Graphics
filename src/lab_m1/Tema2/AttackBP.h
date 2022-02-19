#pragma once
#include "BPickup.h"

class AttackBP :public BPickup {
public:
	AttackBP();
	AttackBP(glm::vec3 pos);
	~AttackBP();
	void applyAction(float& health, Character& player);

private:
	int type;
};