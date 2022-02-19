#pragma once
#include <core/gpu/mesh.h>
#include "myobject2D.h"
#include "transform2D.h"
#include "randUtils.h"
#include "Healthbar.h"
#include "Player.h"

class Pickup {
public:
	Pickup();
	Pickup(float width,float height,glm::vec3 color);
	~Pickup();
	glm::vec2 GetPos();
	float GetRadius();
	glm::mat3 GetModelMatrix();
	Mesh* GetMesh();
	virtual void applyAction(HealthBar* hb,Player& player);

protected:
	Mesh* mesh;
	glm::vec2 pos;
	float radius;
};
