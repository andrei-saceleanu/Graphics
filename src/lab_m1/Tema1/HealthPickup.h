#pragma once
#include "Pickup.h"

class HealthPickup :public Pickup {
public:
	HealthPickup();
	HealthPickup(float width,float height);
	void applyAction(HealthBar* hb,Player& player);
};