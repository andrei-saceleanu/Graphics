#include "RayMarch.h"

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


RayMarch::RayMarch()
{
    p = glm::vec3(0, 1, 0);
}


RayMarch::~RayMarch()
{
}


void RayMarch::Init()
{

    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;

    camera = new ray::Camera();
    camera->Set(glm::vec3(0, 0, 3), glm::vec3(0), glm::vec3(0, 1, 0));
    
	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
	meshes[mesh->GetMeshID()] = mesh;
    

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "RayMarch", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "RayMarch", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    


    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    projectionMatrix = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, Z_NEAR, Z_FAR);

}




void RayMarch::FrameStart()
{

}


void RayMarch::Update(float deltaTimeSeconds)
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, 3.1415f / 2, glm::vec3(-1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f / 50, 1.f, 1.f / 50));
    modelMatrix = glm::translate(modelMatrix,glm::vec3(25, 0, 25));
    RenderSimpleMeshGeneral(meshes["plane"], shaders["MyShader"], modelMatrix, camera->GetViewMatrix(), projectionMatrix, glm::vec3(1, 0, 0));
}


void RayMarch::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}






void RayMarch::RenderSimpleMeshGeneral(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& vizMatrix, const glm::mat4& projectionMatrix, const glm::vec3& color,int type)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);


    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));


    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(vizMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int timeLocation = glGetUniformLocation(shader->GetProgramID(), "iTime");
    glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

    int typeLocation = glGetUniformLocation(shader->GetProgramID(), "Type");
    glUniform1i(typeLocation, (GLint)type);

    glm::vec2 res = window->GetResolution();
    glUniform2fv(glGetUniformLocation(shader->GetProgramID(), "res"),1,glm::value_ptr(res));

    glUniform3fv(glGetUniformLocation(shader->GetProgramID(),"cameraPos"),1,glm::value_ptr(p));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

}




void RayMarch::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void RayMarch::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    
	float cameraSpeed = 3.0f;
    glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));


	if (window->KeyHold(GLFW_KEY_W)) 
    {
		//camera->MoveForward(deltaTime * cameraSpeed);
        p -= deltaTime * cameraSpeed * dir;
        
	}
	if (window->KeyHold(GLFW_KEY_A))
	{
		//camera->TranslateRight(-deltaTime * cameraSpeed);
        p -= deltaTime * cameraSpeed * camera->right;
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		//camera->MoveForward(-deltaTime * cameraSpeed);
        p += deltaTime * cameraSpeed * dir;
	}
	if (window->KeyHold(GLFW_KEY_D))
	{
		//camera->TranslateRight(deltaTime * cameraSpeed);
        p += deltaTime * cameraSpeed * camera->right;
	}
}


void RayMarch::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void RayMarch::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void RayMarch::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

            
        camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
        camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        
        
    }
}


void RayMarch::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

}


void RayMarch::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void RayMarch::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void RayMarch::OnWindowResize(int width, int height)
{
}
