#include "Missile.h"
#include <lab_m1/Tema2/collisionUtils.h>

Missile::Missile(glm::vec3 pos, glm::vec3 direction)
{
	float speedMagnitude = 1.f;
	radius = 1.f;
	lifeSpan = 0.0f;
	v = speedMagnitude * direction; //initial speed
	acc = glm::vec3(0.f);
	this->pos = pos;
	exploding = false;
}

Missile::~Missile()
{
}

glm::vec3 Missile::GetPos()
{
	return pos;
}

glm::vec3 Missile::GetSpeed()
{
	return v;
}

float Missile::GetLifeSpan()
{
	return lifeSpan;
}

float Missile::GetRadius()
{
	return radius;
}

bool Missile::getExplodingStatus()
{
	return exploding;
}

void Missile::setExplodingStatus(bool status)
{
	this->exploding = status;
}

void Missile::ChangeRadius(float delta)
{
	radius += delta;
}

void Missile::ChangeLifeSpan(float delta)
{
	lifeSpan += delta;
}

glm::mat4 Missile::GetModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));
	return modelMatrix;
}

void Missile::UpdatePos(float deltaTime)
{
	v += acc;
	pos += v;
	acc *= 0.f;
	ChangeLifeSpan(deltaTime);
}

void Missile::applyForce(glm::vec3 force)
{
	acc += force;
}



bool Missile::checkWalls(int** mazePattern, int mazeSize, float cellSize)
{
	int ix = pos.z / cellSize;
	int iy = pos.x / cellSize;
	int dx[] = { -1,0,1,0 };
	int dy[] = { 0,1,0,-1 };
	for (int k = 0; k < 4; k++) {
		if (ix + dx[k] >= 0 && ix + dx[k] < mazeSize && iy + dy[k] >= 0 && iy + dy[k] < mazeSize && mazePattern[ix + dx[k]][iy + dy[k]] == 0) {
			glm::vec3 boxPos = glm::vec3(cellSize * (iy + dy[k]),0, cellSize * (ix + dx[k]));
			glm::vec3 boxDim = glm::vec3(cellSize,5.f, cellSize);
			if (collisionUtils::sphereBoxCollision(pos, radius, boxPos, boxDim)) {
				return true;
			}
		}
	}
	return false;
}
