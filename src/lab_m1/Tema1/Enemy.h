#pragma once
#include "utils/glm_utils.h"
#include "collision.h"
#include "Projectile.h"
#include <vector>
#include <core/gpu/mesh.h>
#include "myobject2D.h"
#include "transform2D.h"
#include "randUtils.h"

class Enemy {
public:
	Enemy();
	Enemy(float width, float height,float speed,int id);
	~Enemy();
	Mesh* GetMesh();
	glm::vec2 GetPos();
	glm::vec2 GetSecPos(int select);
	glm::mat3 GetMainMatrix();
	glm::mat3 GetSecMatrix(int select);
	glm::vec4 GetHitBox();
	glm::vec2 GetMainSize();
	glm::vec2 GetSecSize();
	void UpdatePos();
	bool CheckBoundary(float width,float height);
	bool checkProjectiles(std::vector<Projectile>& projectiles);
	bool checkHitBox(glm::vec3 hitBox);
	void SeekPlayer(glm::vec2 pos,std::vector<Enemy> otherEnemies,std::vector<Projectile> projectiles);
	int GetID();
private:
	void keepDistanceFromOthers(std::vector<Enemy> otherEnemies);
	void avoidProjectiles(std::vector<Projectile> projectiles);

	Mesh* mainShape;
	glm::vec2 pos, v, acc;
	int id;
	float angle;
	float mainSize, secSize, offset;
	float maxSpeed, maxForce;
	float sensingRadius;
	float avoidAbility;
};