#pragma once

#include "components/simple_scene.h"
#include "camera.h"
#include "randomUtils.h"

#include <vector>
#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class RayMarch : public gfxc::SimpleScene
    {
     public:
        RayMarch();
        ~RayMarch();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void RenderSimpleMeshGeneral(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& vizMatrix, const glm::mat4& projectionMatrix, const glm::vec3& color = glm::vec3(1),int type = -1);

        
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        ray::Camera* camera,*camera2;
        glm::mat4 projectionMatrix;
        glm::vec3 p;
        glm::vec2 mouse;
        glm::vec3 f, r, u;
        float z;
        

        float right;
        float left;
        float bottom;
        float top;
        float fov;
    };
}   // namespace m1
