#pragma once
#include "utils/glm_utils.h"
#include <vector>

class Missile {
public:
	Missile(glm::vec3 pos, glm::vec3 direction);
	~Missile();
	glm::vec3 GetPos();
	glm::vec3 GetSpeed();
	float GetLifeSpan();
	float GetRadius();
	bool getExplodingStatus();
	void setExplodingStatus(bool status);
	void ChangeRadius(float delta);
	void ChangeLifeSpan(float delta);
	glm::mat4 GetModelMatrix();
	void UpdatePos(float deltaTime);
	void applyForce(glm::vec3 force);
	bool checkWalls(int** mazePattern, int mazeSize, float cellSize);



private:
	glm::vec3 pos, v, acc;
	float lifeSpan, radius;
	bool exploding;
};