#include "HealthPickup.h"

HealthPickup::HealthPickup()
{
}

HealthPickup::HealthPickup(float width, float height):Pickup(width,height,glm::vec3(0,1,0))
{
}

void HealthPickup::applyAction(HealthBar* hb,Player& player)
{
	hb->changeScale(20);
}
