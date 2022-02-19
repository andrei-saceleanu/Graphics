#include "DefensePickup.h"

DefensePickup::DefensePickup()
{
}

DefensePickup::DefensePickup(float width, float height):Pickup(width, height, glm::vec3(0, 1, 1))
{
}

void DefensePickup::applyAction(HealthBar* hb, Player& player)
{
	player.SetHitBoxStatus(true);
}
