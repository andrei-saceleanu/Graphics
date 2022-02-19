#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

float degrees = -atan(3);
float radius = sqrt(10);
Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    r = 0;
    g = 0;
    b = 0;
    cnt = 0;
    pos = glm::vec3(3, 3, 3);
    v0 = 2;
    acc = -1;
    t = 0;
    ang = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    
     Mesh* mesh = new Mesh("box");
     mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
     meshes[mesh->GetMeshID()] = mesh;
    

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
     Mesh* mesh1 = new Mesh("sphere");
     mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
     meshes[mesh1->GetMeshID()] = mesh1;
     Mesh* mesh2 = new Mesh("teapot");
     mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
     meshes[mesh2->GetMeshID()] = mesh2;
}


void Lab1::FrameStart()
{
}
glm::vec3 p = glm::vec3(1, 0.5f, -3);

void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(r, g, b, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(meshes["sphere"], glm::vec3(1, 1, 1),glm::vec3(0.75f));
    RenderMesh(meshes["teapot"], glm::vec3(-1, 0, -3), glm::vec3(0.75f));

    if (cnt == 0) {
        RenderMesh(meshes["box"], glm::vec3(3, 3, -2));
    }
    else if (cnt == 1) {
        RenderMesh(meshes["sphere"], glm::vec3(3, 3, -2));
    }
    else {
        RenderMesh(meshes["teapot"], glm::vec3(3, 3, -2));
    }
    RenderMesh(meshes["box"], pos);
    //myRender(meshes["box"], p,ang);
    t += deltaTimeSeconds;
    ang += 0.1;
    p.y = 0.5f + v0 * t + 0.5f * acc * t * t;
    if (p.y <= 0.5f) {
        t = 0;
    }

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (!window->MouseHold(1)) { // avoid moving camera and object simultaneously
        if (window->KeyHold(GLFW_KEY_W)) {
            pos.y += 5*deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            pos.y -= 5 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            pos.x -= 5 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            pos.x += 5 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            pos.z -= 5 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_E)) {
            pos.z += 5 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_R)) {
            degrees += 5 * deltaTime;
            p.x = radius * cos(degrees);
            p.z = radius * sin(degrees);

        }
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        srand(time(NULL));
        r = (float)rand() / (float)(RAND_MAX);
        g = (float)rand() / (float)(RAND_MAX);
        b = (float)rand() / (float)(RAND_MAX);

    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_O) {
        cnt += 1;
        if (cnt >= 3)
            cnt = 0;
    }
    
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
