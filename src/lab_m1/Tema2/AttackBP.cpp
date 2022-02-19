#include "AttackBP.h"
#include "randomUtils.h"

AttackBP::AttackBP()
{
}

AttackBP::AttackBP(glm::vec3 pos):BPickup(pos)
{
	this->color = glm::vec3(1.f, 0.f, 0.f);
	type = randomUtils::randIntervalI(0, 1);
}

AttackBP::~AttackBP()
{
}

void AttackBP::applyAction(float& health, Character& player)
{
	if (type == 0) {
		player.setMultipleProj(true);
	}
	else if (type == 1) {
		player.setExplodingProj(true);
	}
}
