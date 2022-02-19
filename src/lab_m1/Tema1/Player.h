#pragma once
#include <core/gpu/mesh.h>
#include "Obstacle.h"
#include "Enemy.h"


class Player {
public:
	Player();
	~Player();

	float GetBodyRadius();
	float GetFrontRadius();
	float GetThrusterRadius();
	float GetAngle();

	Mesh* GetBodyMesh();
	Mesh* GetFrontMesh();
	Mesh* GetLeftThrusterMesh();
	Mesh* GetRightThrusterMesh();
	Mesh* GetHitBoxMesh();

	glm::vec2 GetPos();
	glm::vec2 GetFrontPos();
	glm::vec2 GetDir();
	glm::vec2 GetThrusterPos(int select,float angle=-1000);
	glm::vec3 GetHitBox();
	bool GetHitBoxStatus();
	void SetX(float newOffsetX);
	void SetY(float newOffsetY);
	void SetHitBoxStatus(bool newValue);
	void SetAngle (float newAng);
	void changeX(float delta);
	void changeY(float delta);
	bool checkAngleAtBoundary(float angle,float width,float height);
	bool checkBoundary(float width,float height);
	bool checkCollisionObstacle(std::vector<Obstacle> obstacles);
	bool checkCollisionAngleObstacle(std::vector<Obstacle> obstacles,float angle);
	bool checkCollisionEnemy(Enemy e);
	

	glm::mat3 GetBodyMatrix();
	glm::mat3 GetFrontMatrix();
	glm::mat3 GetThrusterMatrix(int select);
	glm::mat3 GetHitBoxMatrix();
	
	
private:
	bool hitBoxActive;
	float cannonAngle;
	float phi1, phi2;
	float playerOffsetX, playerOffsetY;
	float bodyRadius, frontRadius, thrusterRadius, hitBoxRadius;
	float frontOffsetX, frontOffsetY;
	float rearOffset, lateralOffset;
	float r;
	Mesh* bodyMesh;
	Mesh* frontMesh;
	Mesh* leftThrusterMesh;
	Mesh* rightThrusterMesh;
	Mesh* hitBox;
};
