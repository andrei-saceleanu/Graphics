#include "HealthBP.h"

HealthBP::HealthBP()
{
}

HealthBP::HealthBP(glm::vec3 pos):BPickup(pos)
{
	this->color = glm::vec3(0.f, 1.f, 0.f);
}

HealthBP::~HealthBP()
{
}

void HealthBP::applyAction(float& health,Character& player) {
	health = (health + 0.05f <= 1.f) ? (health + 0.05f) : 1.f;
}
