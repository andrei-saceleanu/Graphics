#pragma once

#include "components/simple_scene.h"
#include "Player.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "Enemy.h"
#include "lab_m1/Tema1/Healthbar.h"
#include "randUtils.h"
#include "Pickup.h"
#include "HealthPickup.h"
#include "DefensePickup.h"
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void putObstaclesInScene();
        void spawnEnemies();
        void DrawScene(glm::mat3 visMatrix);
        void RenderPlayer(glm::mat3 visMatrix);
        void RenderBoundary(glm::mat3 visMatrix);
        void RenderObstacles(glm::mat3 visMatrix);
        void RenderProjectiles(glm::mat3 visMatrix);
        void MoveProjectiles(float deltaTime);
        void RenderHealthbar(glm::mat3 visMatrix);
        void RenderEnemies(glm::mat3 visMatrix);
        void RenderPickups(glm::mat3 visMatrix);
        void RenderScore(glm::mat3 visMatrix);
        void MoveEnemies();
        void removeObstacleCells(Obstacle o);

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

     protected:
        void checkForPickups();
        glm::mat3 modelMatrix, visMatrix;
        float sceneSize;
        float fireRate;
        float globalTime;
        float squareSide;
        float speed;
        float enemyTimeUnit, pickupTimeUnit, defenseTimeUnit;
        float maxDistanceProjectile;
        int newEnemyCount,initialEnemyCount, maxEnemyIndex;
        int score;
        int cntObstacles;
        bool gameOver;
        int cellSize;
        unsigned char** occupied;

        HealthBar* healthbar;
        std::vector<Mesh*> v;
        Player player;
        ViewportSpace viewSpace, viewSpaceMinimap;
        LogicSpace logicSpace, logicSpaceMinimap;
        std::vector<Obstacle> obstacles;
        std::vector<Projectile> projectiles;
        std::vector<Enemy> enemies;
        std::vector<Pickup*> pickups;
        

    };
}   // namespace m1
