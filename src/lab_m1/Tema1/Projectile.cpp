#include "Projectile.h"
#include <lab_m1/Tema1/transform2D.h>

Projectile::Projectile(glm::vec2 pos, glm::vec2 direction, float speed)
{
	this->pos = pos;
	this->direction = direction;
	this->speed = speed;
	this->radius = 10;
	this->distanceTravelled = 0.0f;
}

Projectile::~Projectile()
{
}

glm::vec2 Projectile::GetPos()
{
	return pos;
}

glm::vec2 Projectile::GetDirection()
{
	return direction;
}

float Projectile::GetSpeed()
{
	return speed;
}

float Projectile::GetDistanceTravelled()
{
	return distanceTravelled;
}

float Projectile::GetRadius()
{
	return radius;
}

void Projectile::ChangeRadius(float delta)
{
	radius += delta;
}

void Projectile::ChangeDistanceTravelled(float delta)
{
	distanceTravelled += delta;
}

void Projectile::ChangeSpeed(float delta)
{
	speed += delta;
}

glm::mat3 Projectile::GetModelMatrix()
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(pos.x, pos.y);
	modelMatrix *= transform2D::Scale(radius, radius);
	return modelMatrix;
}

void Projectile::UpdatePos(float deltaTime)
{
	float magnitude = speed * deltaTime;
	pos = pos + magnitude * direction;
	ChangeDistanceTravelled(magnitude);
}

bool Projectile::checkObstacles(std::vector<Obstacle>& obstacles)
{
	for (Obstacle& o : obstacles) {
		if (collision::circleRectCollision(this->GetPos(), this->GetRadius(), o.GetPosition(), o.GetDimensions())) {
			return false;
		}
	}
	return true;
}
