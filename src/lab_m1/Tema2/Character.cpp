#include "Character.h"
#include <lab_m1/Tema2/collisionUtils.h>


Character::Character() {
	pos = glm::vec3(0, 1, 0);
	shieldActive = false;
	multipleProjectiles = false;
	explodingProjectile = false;
}

Character::~Character() {

}

Character::Character(Mesh* playerPart) {
	this->playerPart = new Mesh(*playerPart);
}

glm::vec3 Character::getPos() {
	return pos;
}

void Character::setPos(glm::vec3 pos) {
	this->pos = pos;
}

Mesh* Character::getCharacterPartMesh()
{
	return playerPart;
}

glm::mat4 Character::getPlayerPartModelMatrix(int select)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	if (select == 0) { // body
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.f / 4));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f/2));
	}
	else if (select == 1) { // head
		modelMatrix = glm::translate(modelMatrix, pos+glm::vec3(0,0.375f,0));
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));
	}
	else if (select == 2) { // left arm
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.375f, 0.125f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));
	}
	else if (select == 3) { // right arm
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.375f, 0.125f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));

	}
	else if (select == 4) { // left forearm
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.375f, -0.125f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));
	}
	else if (select == 5) { // right forearm
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.375f, -0.125f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));

	}
	else if (select == 6) { // left leg
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.125f, -0.5f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.175f / 2, -1.f / 2, -0.175f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.175f,1.f,0.175f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));
	}
	else if (select == 7) { // right leg
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.125f, -0.5f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.175f / 2, -1.f / 2, -0.175f / 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.175f, 1.f, 0.175f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));
	}
	return modelMatrix;
}

glm::mat4 Character::getHitBoxMatrix()
{
	glm::vec4 leftArm1 = this->getPlayerPartModelMatrix(2) * glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	glm::vec4 leftArm2 = this->getPlayerPartModelMatrix(2) * glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	glm::vec4 rightArm1 = this->getPlayerPartModelMatrix(3) * glm::vec4(0.5f, 0.5f, 0.5f, 1);
	glm::vec4 rightArm2 = this->getPlayerPartModelMatrix(3) * glm::vec4(0.5f, 0.5f, -0.5f, 1);
	glm::vec4 bodyArm1 = this->getPlayerPartModelMatrix(0) * glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	glm::vec4 bodyArm2 = this->getPlayerPartModelMatrix(0) * glm::vec4(-0.5f, 0.5f, -0.5f, 1);

	float minx = glm::min(leftArm1.x, glm::min(leftArm2.x, glm::min(rightArm1.x, glm::min(rightArm2.x, glm::min(bodyArm1.x, bodyArm2.x)))));
	float maxx = glm::max(leftArm1.x, glm::max(leftArm2.x, glm::max(rightArm1.x, glm::max(rightArm2.x, glm::max(bodyArm1.x, bodyArm2.x)))));
	float minz = glm::min(leftArm1.z, glm::min(leftArm2.z, glm::min(rightArm1.z, glm::min(rightArm2.z, glm::min(bodyArm1.z, bodyArm2.z)))));
	float maxz = glm::max(leftArm1.z, glm::max(leftArm2.z, glm::max(rightArm1.z, glm::max(rightArm2.z, glm::max(bodyArm1.z, bodyArm2.z)))));
	float sx = maxx - minx + 0.1f;
	float sz = maxz - minz + 0.1f;

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x,0.75f,pos.z));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-sx/2, -1.55f/2, -sz/2));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(sx,1.55f,sz));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f / 2));

	return modelMatrix;
}

glm::vec4 Character::getHitBox()
{
	glm::vec4 leftArm1 = this->getPlayerPartModelMatrix(2) * glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	glm::vec4 leftArm2 = this->getPlayerPartModelMatrix(2) * glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	glm::vec4 rightArm1 = this->getPlayerPartModelMatrix(3) * glm::vec4(0.5f, 0.5f, 0.5f, 1);
	glm::vec4 rightArm2 = this->getPlayerPartModelMatrix(3) * glm::vec4(0.5f, 0.5f, -0.5f, 1);
	glm::vec4 bodyArm1 = this->getPlayerPartModelMatrix(0) * glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	glm::vec4 bodyArm2 = this->getPlayerPartModelMatrix(0) * glm::vec4(-0.5f, 0.5f, -0.5f, 1);

	float minx = glm::min(leftArm1.x, glm::min(leftArm2.x, glm::min(rightArm1.x, glm::min(rightArm2.x, glm::min(bodyArm1.x, bodyArm2.x)))));
	float maxx = glm::max(leftArm1.x, glm::max(leftArm2.x, glm::max(rightArm1.x, glm::max(rightArm2.x, glm::max(bodyArm1.x, bodyArm2.x)))));
	float minz = glm::min(leftArm1.z, glm::min(leftArm2.z, glm::min(rightArm1.z, glm::min(rightArm2.z, glm::min(bodyArm1.z, bodyArm2.z)))));
	float maxz = glm::max(leftArm1.z, glm::max(leftArm2.z, glm::max(rightArm1.z, glm::max(rightArm2.z, glm::max(bodyArm1.z, bodyArm2.z)))));
	float sizeX = maxx - minx + 0.1f;
	float sizeZ = maxz - minz + 0.1f;
	glm::vec2 leftUpperCorner = glm::vec2(pos.x - sizeX / 2, pos.z - sizeZ / 2);
	return glm::vec4(minx,minz,sizeX,sizeZ);
}


bool Character::checkWalls(int** mazePattern,int mazeSize, float cellSize)
{
	int ix = pos.z / cellSize;
	int iy = pos.x / cellSize;
	int dx[] = { -1,0,1,0 };
	int dy[] = { 0,1,0,-1 };
	for (int k=0;k<4;k++){
			if(ix+dx[k]>=0 && ix+dx[k]<mazeSize && iy + dy[k] >= 0 && iy + dy[k] < mazeSize && mazePattern[ix+dx[k]][iy+dy[k]]==0){
				glm::vec2 pos = glm::vec2(cellSize * (iy + dy[k]), cellSize * (ix + dx[k]));
				glm::vec2 dim = glm::vec2(cellSize, cellSize);
				glm::vec4 hb = this->getHitBox();
				glm::vec2 pos2 = glm::vec2(hb.x, hb.y);
				glm::vec2 dim2 = glm::vec2(hb.z, hb.w);
				if (collisionUtils::rectRectCollision(pos, dim, pos2, dim2)) {
					return true;
				}
			}
	}
	return false;
}

bool Character::checkCollisionEnemy(Opponent e)
{
	glm::vec3 enemyPos = e.getPos();
	float enemyRadius = e.getRadius();
	glm::vec4 hb = this->getHitBox();
	glm::vec3 thisPos = glm::vec3(hb.x, 0, hb.y);
	glm::vec3 thisDim = glm::vec3(hb.z,1.5f, hb.w);
	if (collisionUtils::sphereBoxCollision(enemyPos, enemyRadius, thisPos, thisDim))
		return true;
	return false;
}

bool Character::outOfMaze(int cellSize, int mazeSize)
{
	if (pos.x <= 0.f) {
		return true;
	}
	if (pos.z <= 0.f) {
		return true;
	}
	int x = pos.z / cellSize;
	int y = pos.x / cellSize;
	if (x >= mazeSize || y >= mazeSize) {
		return true;
	}
	return false;
}

void Character::changeX(float delta) {
	pos.x += delta;
}

void Character::changeY(float delta) {
	pos.y += delta;
}

void Character::changeZ(float delta) {
	pos.z += delta;
}

void Character::changePos(glm::vec3 delta)
{
	pos += delta;
}

void Character::setAng(float angle)
{
	prev_angle = this->angle;
	this->angle = angle;
}

float Character::getPrevAng()
{
	return prev_angle;
}

bool Character::getShieldStatus()
{
	return shieldActive;
}

void Character::setShieldStatus(bool status)
{
	this->shieldActive = status;
}

bool Character::getMultipleProj()
{
	return multipleProjectiles;
}

void Character::setMultipleProj(bool status)
{
	this->multipleProjectiles = status;
}

bool Character::getExplodingProj()
{
	return explodingProjectile;
}

void Character::setExplodingProj(bool status)
{
	this->explodingProjectile = status;
}




