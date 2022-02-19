#pragma once
#include "utils/glm_utils.h"
#include "Obstacle.h"
#include "collision.h"
#include <vector>

class Projectile {
public:
	Projectile(glm::vec2 pos, glm::vec2 direction, float speed);
	~Projectile();
	glm::vec2 GetPos();
	glm::vec2 GetDirection();
	float GetSpeed();
	float GetDistanceTravelled();
	float GetRadius();
	void ChangeRadius(float delta);
	void ChangeDistanceTravelled(float delta);
	void ChangeSpeed(float delta);
	glm::mat3 GetModelMatrix();
	void UpdatePos(float deltaTime);
	bool checkObstacles(std::vector<Obstacle>& obstacles);

private:
	glm::vec2 pos, direction;
	float speed, distanceTravelled, radius;
};