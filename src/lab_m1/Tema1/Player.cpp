#include "Player.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/myobject2D.h"
#include "lab_m1/Tema1/collision.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>

Player::Player() {
	bodyRadius = 40;
	frontRadius = 25;
	thrusterRadius = 15;

	bodyMesh = myobject2D::CreateCircle("body", bodyRadius, glm::vec3(0.5f, 0.1f, 0.1f), true);
	frontMesh = myobject2D::CreateCircle("front", frontRadius, glm::vec3(0.5f, 0.2f, 0.3f), true);
	leftThrusterMesh = myobject2D::CreateDiscus("leftT", thrusterRadius, glm::vec3(0.5f, 0.2f, 0.3f));
	rightThrusterMesh = myobject2D::CreateDiscus("rightT", thrusterRadius, glm::vec3(0.5f, 0.2f, 0.3f));
	playerOffsetX = 200;
	playerOffsetY = 200;
	rearOffset = 30;
	lateralOffset = 20;
	frontOffsetX = 30;
	frontOffsetY = 0;
	cannonAngle = 0;
	hitBoxRadius = frontOffsetX + frontRadius + 10;
	hitBox = myobject2D::CreateDiscus("hitBox", hitBoxRadius, glm::vec3(0.9f, 0.9f, 0.9f));
	hitBoxActive = false;
	phi1 = atan2(lateralOffset ,- rearOffset);
	phi2 = atan2(-lateralOffset,-rearOffset);
	r = sqrt(rearOffset * rearOffset + lateralOffset *lateralOffset);
}

Player::~Player()
{
}

float Player::GetBodyRadius()
{
	return bodyRadius;
}

float Player::GetFrontRadius()
{
	return frontRadius;
}

float Player::GetThrusterRadius()
{
	return thrusterRadius;
}

float Player::GetAngle()
{
	return cannonAngle;
}

Mesh* Player::GetBodyMesh()
{
	return bodyMesh;
}

Mesh* Player::GetFrontMesh()
{
	return frontMesh;
}

Mesh* Player::GetLeftThrusterMesh()
{
	return leftThrusterMesh;
}

Mesh* Player::GetRightThrusterMesh()
{
	return rightThrusterMesh;
}

Mesh* Player::GetHitBoxMesh()
{
	return hitBox;
}

glm::vec2 Player::GetPos()
{
	return glm::vec2(playerOffsetX, playerOffsetY);
}

glm::vec2 Player::GetFrontPos() {
	return glm::vec2(playerOffsetX+frontOffsetX*cos(cannonAngle),playerOffsetY+frontOffsetX*sin(cannonAngle));
}

glm::vec2 Player::GetDir()
{
	return glm::vec2(cos(cannonAngle),sin(cannonAngle));
}

void Player::SetX(float newOffsetX)
{
	playerOffsetX = newOffsetX;
}

void Player::SetY(float newOffsetY)
{
	playerOffsetY = newOffsetY;
}

void Player::SetHitBoxStatus(bool newValue)
{
	hitBoxActive = newValue;
}

void Player::SetAngle(float newAng)
{
	cannonAngle = newAng;
}

void Player::changeX(float delta)
{
	playerOffsetX += delta;
}

void Player::changeY(float delta)
{
	playerOffsetY += delta;
}

bool Player::checkAngleAtBoundary(float angle,float width,float height)
{
	glm::vec2 cannonPos = this->GetPos() + glm::vec2(frontOffsetX * cos(angle), frontOffsetX * sin(angle));
	glm::vec2 leftPos = this->GetThrusterPos(0, angle);
	glm::vec2 rightPos = this->GetThrusterPos(1, angle);
	bool front = collision::circleWithinBoundary(cannonPos, frontRadius,width,height);
	bool left = collision::circleWithinBoundary(leftPos, thrusterRadius,width,height);
	bool right = collision::circleWithinBoundary(rightPos, thrusterRadius,width,height);
	return front && left && right;
}



bool Player::checkBoundary(float width,float height)
{
	glm::vec2 bodyPos = this->GetPos();
	glm::vec2 frontPos = this->GetFrontPos();
	glm::vec2 leftPos = this->GetThrusterPos(0);
	glm::vec2 rightPos = this->GetThrusterPos(1);
	bool body = collision::circleWithinBoundary(bodyPos, bodyRadius,width,height);
	bool front = collision::circleWithinBoundary(frontPos, frontRadius,width,height);
	bool left = collision::circleWithinBoundary(leftPos, thrusterRadius,width,height);
	bool right = collision::circleWithinBoundary(rightPos, thrusterRadius,width,height);
	
	return body && front && left && right;

}

bool Player::checkCollisionObstacle(std::vector<Obstacle> obstacles)
{
	for (Obstacle& obs : obstacles) {
		bool b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b2 = collision::circleRectCollision(this->GetFrontPos(), this->GetFrontRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b3 = collision::circleRectCollision(this->GetThrusterPos(0), this->GetThrusterRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b4 = collision::circleRectCollision(this->GetThrusterPos(1), this->GetThrusterRadius(), obs.GetPosition(), obs.GetDimensions());
		if (b1 || b2 || b3 || b4)
			return true;
	}
	return false;
}

bool Player::checkCollisionAngleObstacle(std::vector<Obstacle> obstacles,float angle)
{
	glm::vec2 x(frontOffsetX * cos(angle), frontOffsetX * sin(angle));
	for (Obstacle& obs : obstacles) {
		bool b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b2 = collision::circleRectCollision(this->GetPos()+x, this->GetFrontRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b3 = collision::circleRectCollision(this->GetThrusterPos(0,angle), this->GetThrusterRadius(), obs.GetPosition(), obs.GetDimensions());
		bool b4 = collision::circleRectCollision(this->GetThrusterPos(1,angle), this->GetThrusterRadius(), obs.GetPosition(), obs.GetDimensions());
		if (b1 || b2 || b3 || b4)
			return true;
	}
	return false;
}

bool Player::checkCollisionEnemy(Enemy e)
{
		glm::vec4 hb = e.GetHitBox();
		glm::vec2 enemyPos(hb.x, hb.y);
		glm::vec2 enemyDim(hb.z, hb.t);
		bool b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), enemyPos,enemyDim);
		bool b2 = collision::circleRectCollision(this->GetFrontPos(), this->GetFrontRadius(), enemyPos,enemyDim);
		bool b3 = collision::circleRectCollision(this->GetThrusterPos(0), this->GetThrusterRadius(), enemyPos,enemyDim);
		bool b4 = collision::circleRectCollision(this->GetThrusterPos(1), this->GetThrusterRadius(), enemyPos,enemyDim);
		if (!b1 && !b2 && !b3 && !b4) {
			return false;
		}
		b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), e.GetPos(),e.GetMainSize());
		b2 = collision::circleRectCollision(this->GetFrontPos(), this->GetFrontRadius(), e.GetPos(),e.GetMainSize());
		b3 = collision::circleRectCollision(this->GetThrusterPos(0), this->GetThrusterRadius(), e.GetPos(),e.GetMainSize());
		b4 = collision::circleRectCollision(this->GetThrusterPos(1), this->GetThrusterRadius(), e.GetPos(),e.GetMainSize());
		if (b1 || b2 || b3 || b4) {
			return true;
		}
		b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), e.GetSecPos(0), e.GetSecSize());
		b2 = collision::circleRectCollision(this->GetFrontPos(), this->GetFrontRadius(), e.GetSecPos(0), e.GetSecSize());
		b3 = collision::circleRectCollision(this->GetThrusterPos(0), this->GetThrusterRadius(), e.GetSecPos(0), e.GetSecSize());
		b4 = collision::circleRectCollision(this->GetThrusterPos(1), this->GetThrusterRadius(), e.GetSecPos(0), e.GetSecSize());
		if (b1 || b2 || b3 || b4) {
			return true;
		}
		b1 = collision::circleRectCollision(this->GetPos(), this->GetBodyRadius(), e.GetSecPos(1), e.GetSecSize());
		b2 = collision::circleRectCollision(this->GetFrontPos(), this->GetFrontRadius(), e.GetSecPos(1), e.GetSecSize());
		b3 = collision::circleRectCollision(this->GetThrusterPos(0), this->GetThrusterRadius(), e.GetSecPos(1), e.GetSecSize());
		b4 = collision::circleRectCollision(this->GetThrusterPos(1), this->GetThrusterRadius(), e.GetSecPos(1), e.GetSecSize());
		if (b1 || b2 || b3 || b4) {
			return true;
		}
		return false;

}


glm::mat3 Player::GetBodyMatrix()
{
	glm::mat3 modelBody = glm::mat3(1);
	modelBody *= transform2D::Translate(playerOffsetX, playerOffsetY);

	return modelBody;
}

glm::mat3 Player::GetFrontMatrix()
{
	glm::mat3 modelFront = glm::mat3(1);

	modelFront *= transform2D::Translate(playerOffsetX, playerOffsetY);
	modelFront *= transform2D::Rotate(cannonAngle);
	modelFront *= transform2D::Translate(frontOffsetX, frontOffsetY);

	return modelFront;
}

glm::mat3 Player::GetThrusterMatrix(int select)
{
	glm::mat3 modelMatrix = glm::mat3(1);
	if (select == 0) {
		modelMatrix *= transform2D::Translate(playerOffsetX, playerOffsetY);
		modelMatrix *= transform2D::Rotate(cannonAngle);
		modelMatrix *= transform2D::Translate(-rearOffset, lateralOffset);
	}
	if (select == 1) {
		modelMatrix *= transform2D::Translate(playerOffsetX, playerOffsetY);
		modelMatrix *= transform2D::Rotate(cannonAngle);
		modelMatrix *= transform2D::Translate(-rearOffset, -lateralOffset);
	}
	return modelMatrix;
}

glm::mat3 Player::GetHitBoxMatrix()
{
	glm::mat3 m = glm::mat3(1);
	m *= transform2D::Translate(playerOffsetX, playerOffsetY);
	return m;
}

glm::vec2 Player::GetThrusterPos(int select,float angle)
{
	glm::vec2 thrusterPos = glm::vec2(1);
	if (select == 0) {
		if(angle!=-1000)
			thrusterPos = this->GetPos() + glm::vec2(r*cos(phi1+angle), r*sin(phi1+angle));
		else
			thrusterPos = this->GetPos() + glm::vec2(r * cos(phi1 + cannonAngle), r * sin(phi1 + cannonAngle));
	}
	else {
		if (angle != -1000)
			thrusterPos = this->GetPos() + glm::vec2(r * cos(phi2 + angle), r * sin(phi2 + angle));
		else
			thrusterPos = this->GetPos() + glm::vec2(r * cos(phi2 + cannonAngle), r * sin(phi2 + cannonAngle));
	}
	return thrusterPos;
}

glm::vec3 Player::GetHitBox()
{
	return glm::vec3(playerOffsetX,playerOffsetY,hitBoxRadius);
}

bool Player::GetHitBoxStatus()
{
	return hitBoxActive;
}

