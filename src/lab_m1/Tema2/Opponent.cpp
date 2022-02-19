#include "Opponent.h"
#include "randomUtils.h"
#include <cmath>


Opponent::Opponent()
{
}

Opponent::Opponent(glm::vec3 center)
{
	this->center = center;
	this->pos = this->center + glm::vec3(2.f, 0.f, 0.f);
	speed = randomUtils::randIntervalF(2.f, 3.f);
	radius = 0.75f;
	maxSpeed = 0.05f;
	t = 0.f;
	time = 0.f;
	seeking = false;
}

Opponent::~Opponent()
{
}

float Opponent::getRadius() {
	return radius;
}

glm::vec3 Opponent::getPos()
{
	return pos;
}

glm::mat4 Opponent::getModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f*radius));
	return modelMatrix;
}

void Opponent::UpdatePos(float deltaTime,glm::vec3 playerPos)
{
	if (seeking == false) {
		t += deltaTime;
		glm::vec3 offset = glm::vec3(cos(speed * t), 0.f, sin(2 * speed * t));
		pos = center + offset;
	}
	else {
		glm::vec3 dir = playerPos - this->getPos();
		dir = maxSpeed*glm::normalize(dir);
		pos = pos + dir;
	}
}

bool Opponent::checkProjectiles(std::vector<Missile>& projectiles)
{
	int index = 0;
	for (Missile& p : projectiles) {
		if (collisionUtils::sphereSphereCollision(pos, radius, p.GetPos(), p.GetRadius())) {
			projectiles.erase(projectiles.begin() + index);
			return true;
		}
		index++;
	}
	return false;
}


