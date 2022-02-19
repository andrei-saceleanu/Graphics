#include "DefenseBP.h"

DefenseBP::DefenseBP()
{
}

DefenseBP::DefenseBP(glm::vec3 pos):BPickup(pos)
{
	this->color = glm::vec3(0.f, 1.f, 1.f);
}

DefenseBP::~DefenseBP()
{
}

void DefenseBP::applyAction(float& health,Character& player)
{
	player.setShieldStatus(true);
}
