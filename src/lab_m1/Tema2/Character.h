#pragma once
#include <core/gpu/mesh.h>
#include <vector>
#include "Opponent.h"




class Character {
public:
	Character();
	Character(Mesh* playerPart);
	~Character();
	glm::vec3 getPos();
	void setPos(glm::vec3 pos);
	Mesh* getCharacterPartMesh();
	glm::mat4 getPlayerPartModelMatrix(int select);
	glm::mat4 getHitBoxMatrix();
	glm::vec4 getHitBox();
	bool checkWalls(int** mazePattern,int mazeSize, float cellSize);
	bool checkCollisionEnemy(Opponent e);
	bool outOfMaze(int cellSize, int mazeSize);

	void changeX(float delta);
	void changeY(float delta);
	void changeZ(float delta);
	void changePos(glm::vec3 delta);
	void setAng(float angle);
	float getPrevAng();
	bool getShieldStatus();
	void setShieldStatus(bool status);
	bool getMultipleProj();
	void setMultipleProj(bool status);
	bool getExplodingProj();
	void setExplodingProj(bool status);


private:
	Mesh* playerPart;
	glm::vec3 pos;
	float angle, prev_angle;
	bool shieldActive, multipleProjectiles, explodingProjectile;
};