#pragma once
#include "utils/glm_utils.h"
#include "Missile.h"
#include "collisionUtils.h"
#include <vector>


class Opponent {
public:
	Opponent();
	Opponent(glm::vec3 center);
	~Opponent();
	float getRadius();
	glm::vec3 getPos();
	glm::mat4 getModelMatrix();
	void UpdatePos(float deltaTime,glm::vec3 playerPos);
	bool checkProjectiles(std::vector<Missile>& projectiles);
	float time;
	bool seeking;

private:
	glm::vec3 pos, center;
	float radius, t, speed, maxSpeed;
};
