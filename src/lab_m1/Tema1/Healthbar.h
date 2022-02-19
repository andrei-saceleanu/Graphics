#pragma once
#include <core/gpu/mesh.h>
#include "utils/glm_utils.h"
#include "myobject2D.h"
#include "transform2D.h"

class HealthBar {
public:
	HealthBar();
	HealthBar(float offsetX,float offsetY);
	~HealthBar();
	Mesh* GetFrameMesh();
	Mesh* GetBodyMesh();
	glm::mat3 GetFrameMatrix();
	glm::mat3 GetBodyMatrix();
	void changeOffsetX(float delta);
	void changeOffsetY(float delta);
	void changeScale(float delta);
	float GetScale();

private:
	Mesh* frame;
	Mesh* body;
	float offsetX, offsetY, scale;
};