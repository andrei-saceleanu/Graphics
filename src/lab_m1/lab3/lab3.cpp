#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
    squareSide = 100;
    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    tx = 0;
    ty = 0;
    posX = 400;
    posY = -400;
    

    ang = 0;
    pos.x = 1000;
    pos.y = 250;
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
 
    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    direction = false;

    

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);
    
    Mesh* car_body = object2D::CreateSquare("car_body", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(car_body);

    Mesh* car_front = object2D::CreateSquare("car_front", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(car_front);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!
    GetSceneCamera()->SetPosition(glm::vec3(tx, ty, 50));
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(150, 250);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented.
        // Remember, the last matrix in the chain will take effect first!
        if (direction) {
            translateX += 100 * deltaTimeSeconds;
            modelMatrix *= transform2D::Translate(translateX, translateY);
            if (translateX > resolution.x * 0.5f)
                direction = false;
        }
        else {
            translateX -= 100 * deltaTimeSeconds;
            modelMatrix *= transform2D::Translate(translateX, translateY);
            if (translateX <= 0)
                direction = true;
        }

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }

    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(resolution.x/2, resolution.y/2);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented.
        // Remember, the last matrix in the chain will take effect first!
        modelMatrix *= transform2D::Translate(tx, ty);
        modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 2);

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
    {
        modelMatrix = glm::mat3(1);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented.
        // Remember, the last matrix in the chain will take effect first!
        modelMatrix *= transform2D::Translate(posX, posY);
        modelMatrix *= transform2D::Translate(-5*squareSide / 2, -squareSide / 2);
        modelMatrix *= transform2D::Scale(5, 1);

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }


    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(400, 250);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented
        // Remember, the last matrix in the chain will take effect first!
        angularStep += deltaTimeSeconds;
        modelMatrix *= transform2D::Translate(squareSide/2,squareSide/2);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(-squareSide/2,-squareSide/2);

        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }

    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(650, 250);
        // TODO(student): Create animations by multiplying the current
        // transform matrix with the matrices you just implemented
        // Remember, the last matrix in the chain will take effect first!
        scaleX += 2 * deltaTimeSeconds;
        scaleY += 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(squareSide / 2, squareSide / 2);
        modelMatrix *= transform2D::Scale(sin(scaleX), sin(scaleY));
        modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 2);

        RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    }

    //Bonus
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(pos.x, pos.y);
        modelMatrix *= transform2D::Rotate(ang);
        modelMatrix *= transform2D::Translate(-squareSide / 2, -squareSide / 2);

        glm::mat3 modelMatrix2 = glm::mat3(1);
        modelMatrix2 *= transform2D::Translate(pos.x, pos.y);
        modelMatrix2 *= transform2D::Rotate(ang);
        modelMatrix2 *= transform2D::Translate(-squareSide / 2, -squareSide / 2);
        modelMatrix2 *= transform2D::Translate(squareSide, 0);
        modelMatrix2 *= transform2D::Scale(0.3, 1);

        RenderMesh2D(meshes["car_body"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["car_front"], shaders["VertexColor"], modelMatrix2);
    }
    
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W))
    {
        pos.x += 400*deltaTime * cos(ang);
        pos.y += 400*deltaTime * sin(ang);
        ty+=5;
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        pos.x -= 400*deltaTime * cos(ang);
        pos.y -= 400*deltaTime * sin(ang);
        ty-=5;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        ang += 5*deltaTime;
        tx-=5;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        ang -= 5*deltaTime;
        tx+=5;
    }
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_0) {
        glm::ivec2 r = window->GetResolution();
        cout << posX << " " << posY << "\n";
        cout <<  r.x/2+tx<< " " << r.y/2+ty << "\n";
    }
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
        cout << mouseX << " " << mouseY << "\n";
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
