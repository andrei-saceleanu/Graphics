#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include <unordered_map>
#include <string>
#include <utility>

#define ANGLE_SPEEDUP		(30.f)

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1),int type = 0
            ,glm::vec2 pos=glm::vec2(1),Texture2D* t=NULL,Texture2D* t2=NULL);
        Texture2D* CreateRandomTexture(unsigned int id,unsigned int width, unsigned int height);
        void RenderFractal(glm::vec3 offset,float scale);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::unordered_map<int, std::pair<float,int> > follow;
        float cellSize;
        int floorSize;
        glm::vec3** colors, ** new_colors;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        float materialKe;
        glm::vec3 ceilLightPos[4], ceilLightDir[4],ceilLightColor[4];
        glm::vec3 currentCeilColor[4], nextCeilColor[4];
        glm::vec3 bezier[9][7][7];
        float cutoffAngle;
        float t, alpha;
        float startTransitionTime;
        float startJumpingTime[5];
        bool jumping[5];
        int sgn;
        int show;
    };
}   // namespace m1
