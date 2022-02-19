#pragma once

#include "components/simple_scene.h"
#include "camera.h"
#include "Character.h"
#include "randomUtils.h"
#include "Missile.h"
#include "Opponent.h"
#include "BPickup.h"
#include "HealthBP.h"
#include "DefenseBP.h"
#include "AttackBP.h"
#include <vector>
#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void GenerateMaze(int regenerate=0);
        void RenderPlayer(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderMaze(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderProjectiles(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderFloor(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderEnemies(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderPickups(glm::mat4 vizMatrix, glm::mat4 projMatrix);
        void RenderHealth();
        void RenderTimeLeft();
        void RenderSimpleMeshGeneral(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& vizMatrix, const glm::mat4& projectionMatrix, const glm::vec3& color = glm::vec3(1),int type = -1);

        void DrawScene(glm::mat4 vizMatrix,glm::mat4 projMatrix);
        void MoveProjectiles(float deltaTime);
        void MoveEnemies(float deltaTime);
        bool OverlapCell(glm::vec3 pos, glm::vec3 pos2);
        void checkForPickups();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        game::Camera* camera, * camera2, *camera3;
        glm::mat4 projectionMatrix, projectionMatrix2, projectionMatrix3;
        float angle;
        glm::vec3 pos;
        int cameraType;
        Character player;
        int** mazePattern;
        int mazeSize;
        float cellSize;
        std::vector<Missile> projectiles;
        std::vector<int> availablePositions;
        std::vector<Opponent> enemies;
        std::vector<Opponent> hitEnemies;
        std::vector<BPickup*> pickups;
        float timeLimit;
        int gameOver, print;
        float health;
        int win;
        float shieldActivationTime, attackActivationTime;
        int regenerate;

        float right;
        float left;
        float bottom;
        float top;
        float fov;
    };
}   // namespace m1
