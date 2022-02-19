#pragma once
#include "Pickup.h"

class DefensePickup :public Pickup {
public:
	DefensePickup();
	DefensePickup(float width, float height);
	void applyAction(HealthBar* hb, Player& player);
};
