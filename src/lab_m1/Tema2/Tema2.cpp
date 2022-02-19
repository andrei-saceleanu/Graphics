#include "Tema2.h"

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


Tema2::Tema2()
{
    timeLimit = 300.f;
    health = 1.f;
    win = 0;
    gameOver = 0;
    print = 1;
    mazeSize = 20;
    regenerate = 1;
    cellSize = 6.f;
    mazePattern = (int**)malloc(mazeSize * sizeof(int*));
    for (int i = 0; i < mazeSize; i++) {
        mazePattern[i] = (int*)calloc(mazeSize, sizeof(int));
    }
    srand(time(NULL));
    GenerateMaze();
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;
    angle = 0.0f;
    pos = glm::vec3(0.f, 1.f, 0.f);
    cameraType = 0; // 0-THIRD_PERSON,1-FIRST_PERSON
    


    camera = new game::Camera();
    camera->Set(player.getPos()+glm::vec3(0.f,0.f,3.f), player.getPos(), glm::vec3(0.f, 1.f, 0.f));

    camera2 = new game::Camera();
    camera2->Set(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));


    camera3 = new game::Camera();
    camera3->Set(glm::vec3(mazeSize*cellSize/2, 10.f, mazeSize * cellSize / 2), glm::vec3(mazeSize * cellSize / 2, 0.f, mazeSize * cellSize / 2), glm::vec3(1.f,0.f, 0.f));

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
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("AlphaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader(Alpha).glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DisplacementShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader(Displacement).glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    projectionMatrix2 = glm::ortho(-1.51f, 1.51f, -1.01f, 1.01f, Z_NEAR, Z_FAR);
    projectionMatrix3 = glm::ortho(-60.f, 60.f, -60.f, 60.f, Z_NEAR, Z_FAR);

}

void Tema2::GenerateMaze(int regenerate)
{
    int** viz = (int**)malloc(mazeSize * sizeof(int*));
    for (int i = 0; i < mazeSize; i++) {
        viz[i] = (int*)calloc(mazeSize, sizeof(int));
    }
    for (int i = 0; i < mazeSize; i++) {
        viz[i][0] = 1;
        viz[0][i] = 1;
        viz[mazeSize - 1][i] = 1;
        viz[i][mazeSize - 1] = 1;
    }
    int dx[] = { -1,0,1,0 };
    int dy[] = { 0,1,0,-1 };
    vector<int> neighbors;
    int nx, ny;
    int nextCell;
    int firstPass = 1;
    int noPasses = 20;
    if (regenerate == 1) {
        availablePositions.clear();
        enemies.clear();
        pickups.clear();
        for (int i = 0; i < mazeSize; i++) {
            for (int j = 0; j < mazeSize; j++) {
                mazePattern[i][j] = 0;
            }
        }
    }
    while (noPasses > 0) {
        int startingCell = randomUtils::randIntervalI(0, mazeSize * mazeSize - 1);
        int x = startingCell / mazeSize;
        int y = startingCell % mazeSize;

        if (firstPass == 0) {
            while (viz[x][y] == 0 || (x == 0 || y == 0 || x == mazeSize - 1 || y == mazeSize - 1)) {
                startingCell = randomUtils::randIntervalI(0, mazeSize * mazeSize - 1);
                x = startingCell / mazeSize;
                y = startingCell % mazeSize;
            }

        }
        else {
            if (regenerate == 0) {
                while (viz[x][y] == 1) {
                    startingCell = randomUtils::randIntervalI(0, mazeSize * mazeSize - 1);
                    x = startingCell / mazeSize;
                    y = startingCell % mazeSize;
                }
            }
            else {
                glm::vec3 pos = player.getPos();
                int x = pos.z / cellSize;
                int y = pos.x / cellSize;
                startingCell = x*mazeSize+y;
            }
        }
        int currentCell = startingCell;
        while (true) {
            x = currentCell / mazeSize;
            y = currentCell % mazeSize;
            viz[x][y] = 1;
            mazePattern[x][y] = 1;
            availablePositions.push_back(mazeSize * x + y);
            for (int k = 0; k < 4; k++) {
                nx = x + dx[k];
                ny = y + dy[k];
                if (nx >= 0 && nx < mazeSize && ny >= 0 && ny < mazeSize) {
                    if (viz[nx][ny] == 0) {
                        neighbors.push_back(nx * mazeSize + ny);
                    }
                    viz[nx][ny] = 1;
                }
            }
            if (neighbors.size() == 0) {
                break;
            }
            nextCell = neighbors[randomUtils::randIntervalI(0, neighbors.size() - 1)];
            currentCell = nextCell;
            neighbors.clear();
        }
        firstPass = 0;
        noPasses--;
    }
    int f = 0;
    int xi, yi;
    if (regenerate == 0) {
        for (int i = mazeSize / 3; i < 2 * mazeSize / 3; i++) {
            for (int j = mazeSize / 3; j < 2 * mazeSize / 3; j++) {
                if (mazePattern[i][j] == 1) {
                    xi = i;
                    yi = j;
                    availablePositions.erase(find(availablePositions.begin(), availablePositions.end(), mazeSize * xi + yi));
                    player.setPos(glm::vec3(cellSize * j + cellSize / 2, 1.f, cellSize * i + cellSize / 2));
                    f = 1;
                    break;
                }
            }
            if (f)
                break;
        }
    }
    else {
        glm::vec3 pos = player.getPos();
        int xi = pos.z / cellSize;
        int yi = pos.x / cellSize;
        availablePositions.erase(find(availablePositions.begin(), availablePositions.end(), mazeSize * xi + yi));
    }
    for (int i = 1; i < mazeSize - 1; i++) { //N
        if (mazePattern[1][i] == 1) {
            mazePattern[0][i] = 1;
            break;
        }
    }
    for (int i = 1; i < mazeSize - 1; i++) { //V
        if (mazePattern[i][1] == 1) {
            mazePattern[i][0] = 1;
            break;
        }
    }
    for (int i = 1; i < mazeSize - 1; i++) { // S
        if (mazePattern[mazeSize - 2][i] == 1) {
            mazePattern[mazeSize - 1][i] = 1;
            break;
        }
    }
    for (int i = 1; i < mazeSize - 1; i++) { // E
        if (mazePattern[i][mazeSize - 2] == 1) {
            mazePattern[i][mazeSize - 1] = 1;
            break;
        }
    }
    int noEnemies = 0.3f * availablePositions.size();
    for (int i = 0; i < noEnemies; i++) {
        int index = randomUtils::randIntervalI(0, availablePositions.size() - 1);
        int pos = availablePositions[index];
        int z = pos / mazeSize;
        int x = pos % mazeSize;
        Opponent newEnemy(glm::vec3(cellSize* x + cellSize / 2, 1.f, cellSize* z + cellSize / 2));
        enemies.push_back(newEnemy);
        availablePositions.erase(availablePositions.begin() + index);
    }
    int noPickups = 0.1f * availablePositions.size();
    float choice;
    BPickup* p;
    for (int i = 0; i < noPickups; i++) {
        int index = randomUtils::randIntervalI(0, availablePositions.size() - 1);
        int pos = availablePositions[index];
        int z = pos / mazeSize;
        int x = pos % mazeSize;
        choice = randomUtils::randIntervalF(0.f, 1.f);
        if(choice<=0.33f)
            p = new DefenseBP(glm::vec3(cellSize * x + cellSize / 2, 1.f, cellSize * z + cellSize / 2));
        else if(choice<=0.66f)
            p = new HealthBP(glm::vec3(cellSize * x + cellSize / 2, 1.f, cellSize * z + cellSize / 2));
        else
            p = new AttackBP(glm::vec3(cellSize * x + cellSize / 2, 1.f, cellSize * z + cellSize / 2));

        pickups.push_back(p);
        availablePositions.erase(availablePositions.begin() + index);
    }

}


void Tema2::FrameStart()
{
    float time = Engine::GetElapsedTime();
    int t = (int)floor(time);
    if (t>=60 && t % 60 == 0) {
        if (regenerate) {
            GenerateMaze(1);
            regenerate = 0;
        }
    }
    if (t >= 61 && t % 61 == 0) {
        regenerate = 1;
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    float time = Engine::GetElapsedTime();
    if (time <= timeLimit && gameOver == 0) {
        if (player.getShieldStatus() && time - shieldActivationTime >= 60.f) {
            player.setShieldStatus(false);
        }
        if (player.getExplodingProj() && time - attackActivationTime >= 60.f) {
            player.setExplodingProj(false);
        }
        if (player.getMultipleProj() && time - attackActivationTime >= 60.f) {
            player.setMultipleProj(false);
        }


        MoveProjectiles(deltaTimeSeconds);
        MoveEnemies(deltaTimeSeconds);
        
        
        DrawScene(camera->GetViewMatrix(), projectionMatrix);
        if (player.getShieldStatus()) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            RenderSimpleMeshGeneral(meshes["box"], shaders["AlphaShader"], player.getHitBoxMatrix(), camera->GetViewMatrix(), projectionMatrix, glm::vec3(0.945f, 0.898f, 0.725f));
            glDisable(GL_BLEND);
        }
        
        glEnable(GL_SCISSOR_TEST);
        glScissor(6.25f * resolution.x / 8, 0, 1.75f * resolution.x / 8, 2*resolution.y / 8);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        glViewport(6.25f * resolution.x / 8, 0, 1.75f * resolution.x / 8, 2*resolution.y / 8);
        DrawScene(camera3->GetViewMatrix(), projectionMatrix3);
        
        glEnable(GL_SCISSOR_TEST);
        glScissor(6 * resolution.x / 8, 6 * resolution.y / 8, 1.75f * resolution.x / 8, resolution.y / 8);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        glViewport(6 * resolution.x / 8, 6 * resolution.y / 8, 1.75f * resolution.x / 8, resolution.y / 8);
        RenderHealth();

        glEnable(GL_SCISSOR_TEST);
        glScissor(0.5f * resolution.x / 8, 6 * resolution.y / 8, 1.75f * resolution.x / 8, resolution.y / 8);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        glViewport(0.5f * resolution.x / 8, 6 * resolution.y / 8, 1.75f * resolution.x / 8, resolution.y / 8);
        RenderTimeLeft();
    }
    else {
        if (print) {
            if (win == 0)
                cout << "GAME OVER!\n";
            else
                cout << "MAZE CLEARED,CONGRATS!\n";
            print = 0;
        }
        gameOver = 1;
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}




void Tema2::RenderPlayer(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    if (vizMatrix == camera3->GetViewMatrix() && projMatrix == projectionMatrix3) {
        glm::mat4 modelMat = glm::mat4(1);
        modelMat = glm::translate(modelMat, glm::vec3(player.getPos()));
        modelMat = glm::scale(modelMat, glm::vec3(2.f));
        RenderSimpleMeshGeneral(meshes["sphere"], shaders["MyShader"], modelMat,vizMatrix,projMatrix, glm::vec3(0.945f, 0.898f, 0.725f));
    }
    if (cameraType == 0) {
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(0),vizMatrix,projMatrix, glm::vec3(0.141f, 0.615f, 0.623f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(1),vizMatrix,projMatrix, glm::vec3(0.945f, 0.898f, 0.725f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(2),vizMatrix,projMatrix, glm::vec3(0.141f, 0.615f, 0.623f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(3),vizMatrix,projMatrix, glm::vec3(0.141f, 0.615f, 0.623f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(4),vizMatrix,projMatrix, glm::vec3(0.945f, 0.898f, 0.725f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(5),vizMatrix,projMatrix, glm::vec3(0.945f, 0.898f, 0.725f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(6),vizMatrix,projMatrix, glm::vec3(0.168f, 0.098f, 0.686f));
        RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], player.getPlayerPartModelMatrix(7),vizMatrix,projMatrix, glm::vec3(0.168f, 0.098f, 0.686f));
    }
}

void Tema2::RenderMaze(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (mazePattern[i][j] == 0) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * j, 0, cellSize * i));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize,5.f,cellSize));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f/2));
                RenderSimpleMeshGeneral(meshes["box"], shaders["MyShader"], modelMatrix,vizMatrix,projMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            }
        }
    }
}

void Tema2::RenderProjectiles(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    for (Missile& m : projectiles) {
        RenderSimpleMeshGeneral(meshes["sphere"], shaders["MyShader"], m.GetModelMatrix(), vizMatrix, projMatrix, glm::vec3(0.2345f, 0.34f, 0.1f));
    }
}

void Tema2::RenderFloor(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100.f, 0.f, 100.f));
    RenderSimpleMeshGeneral(meshes["plane"], shaders["MyShader"], modelMatrix, vizMatrix, projMatrix, glm::vec3(0.2f, 0.3f, 0.4f));
}

void Tema2::RenderEnemies(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    for (Opponent& e : enemies) {
        RenderSimpleMeshGeneral(meshes["sphere"], shaders["DisplacementShader"], e.getModelMatrix(), vizMatrix, projMatrix, glm::vec3(0.8f, 0.1f, 0.7f),1);
    }
    for (Opponent& e : hitEnemies) {
        RenderSimpleMeshGeneral(meshes["sphere"], shaders["DisplacementShader"], e.getModelMatrix(), vizMatrix, projMatrix, glm::vec3(0.8f, 0.1f, 0.7f),0);
    }
}

void Tema2::RenderPickups(glm::mat4 vizMatrix, glm::mat4 projMatrix)
{
    for (int i = 0; i < pickups.size(); i++) {
        RenderSimpleMeshGeneral(meshes["sphere"], shaders["MyShader"], pickups[i]->getModelMatrix(), vizMatrix, projMatrix, pickups[i]->getColor());
    }
}

void Tema2::RenderHealth()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, 1.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 2.f, 1.f));
    modelMatrix = glm::rotate(modelMatrix, 3.14f/2, glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(health*3.f / 50, 1.f, 1.f / 50));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(25.f, 0.f, 25.f));
    RenderSimpleMeshGeneral(meshes["plane"], shaders["MyShader"], modelMatrix,camera2->GetViewMatrix(),projectionMatrix2, glm::vec3(0.f, 1.f, 0.f));
}

void Tema2::RenderTimeLeft()
{
    float time = Engine::GetElapsedTime();
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, 1.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 2.f, 1.f));
    modelMatrix = glm::rotate(modelMatrix, 3.14f / 2, glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(((timeLimit-time)/timeLimit) * 3.f / 50, 1.f, 1.f / 50));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(25.f, 0.f, 25.f));
    RenderSimpleMeshGeneral(meshes["plane"], shaders["MyShader"], modelMatrix,camera2->GetViewMatrix(),projectionMatrix2, glm::vec3(1.f, 0.f, 1.f));

}

void Tema2::RenderSimpleMeshGeneral(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& vizMatrix, const glm::mat4& projectionMatrix, const glm::vec3& color,int type)
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

    int timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
    glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

    int typeLocation = glGetUniformLocation(shader->GetProgramID(), "Type");
    glUniform1i(typeLocation, (GLint)type);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

}

void Tema2::DrawScene(glm::mat4 vizMatrix,glm::mat4 projMatrix)
{
    RenderFloor(vizMatrix,projMatrix);
    RenderPlayer(vizMatrix, projMatrix);
    RenderProjectiles(vizMatrix, projMatrix);
    RenderMaze(vizMatrix, projMatrix);
    RenderEnemies(vizMatrix, projMatrix);
    RenderPickups(vizMatrix, projMatrix);
}

void Tema2::MoveProjectiles(float deltaTime)
{
    vector<Missile> v;
    for (Missile& m : projectiles) {
        m.applyForce(glm::vec3(0, -0.005f, 0));  // gravity
        m.UpdatePos(deltaTime);
        if (m.getExplodingStatus()) {
            if (m.GetLifeSpan() >= 1.f) {
                for (int i = 0; i < 10; i++) {
                    glm::vec3 dir = glm::normalize(glm::vec3(randomUtils::randIntervalF(-5.f, 5.f), randomUtils::randIntervalF(-5.f, 5.f), randomUtils::randIntervalF(-5.f, 5.f)));
                    Missile newProjectile(m.GetPos(), dir);
                    v.push_back(newProjectile);
                }
            }
            else if(!m.checkWalls(mazePattern, mazeSize, cellSize)) {
                v.push_back(m);
            }
        }
        else if (m.GetLifeSpan() <= 2.f && !m.checkWalls(mazePattern,mazeSize,cellSize)) {
            v.push_back(m);
        }
    }
    projectiles.clear();
    projectiles.insert(projectiles.begin(), v.begin(), v.end());

}

void Tema2::MoveEnemies(float deltaTime)
{
    vector<Opponent> v;
    vector<Opponent> v2;
    for (Opponent& e : enemies) {
        bool overlap = OverlapCell(player.getPos(), e.getPos());
        if (overlap)
            e.seeking = true;
        else
            e.seeking = false;
        e.UpdatePos(deltaTime,player.getPos());
        bool c1 = false;
        if(overlap)
            c1 = player.checkCollisionEnemy(e);
        bool c2 = e.checkProjectiles(projectiles);
        if (!c1 && !c2) {
            v.push_back(e);
        }
        else {
            if (c1) {
                if (player.getShieldStatus() == false) {
                    health -= 0.05f;
                    if (health <= 0.f) {
                        gameOver = 1;
                    }
                }
            }
            if (c2) {
                e.time = Engine::GetElapsedTime();
                hitEnemies.push_back(e);  
            }
        }
    }
    for (Opponent& e : hitEnemies) {
        float time = Engine::GetElapsedTime();
        if (time - e.time <= 2.f) {
            v2.push_back(e);
        }
    }
    enemies.clear();
    hitEnemies.clear();
    enemies.insert(enemies.begin(), v.begin(), v.end());
    hitEnemies.insert(hitEnemies.begin(), v2.begin(), v2.end());
}

bool Tema2::OverlapCell(glm::vec3 pos, glm::vec3 pos2)
{
    /* x sunt coloanele din maze,y randurile*/
    int x1 = pos.x / cellSize;
    int y1 = pos.z / cellSize;
    int x2 = pos2.x / cellSize;
    int y2 = pos2.z / cellSize;
    return (x1 == x2) && (y1 == y2);
}

void Tema2::checkForPickups()
{
    glm::vec4 playerHitBox = player.getHitBox();
    glm::vec3 playerPos = glm::vec3(playerHitBox.x, 0.f, playerHitBox.y);
    glm::vec3 playerDims = glm::vec3(playerHitBox.z, 1.5f, playerHitBox.w);
    for (int i = 0; i < pickups.size(); i++) {
        if (collisionUtils::sphereBoxCollision(pickups[i]->getPos(), 0.5f, playerPos, playerDims)) {
            pickups[i]->applyAction(health,player);
            if (player.getShieldStatus()) {
                shieldActivationTime = Engine::GetElapsedTime();
            }
            if (player.getExplodingProj() || player.getMultipleProj()) {
                attackActivationTime = Engine::GetElapsedTime();
            }
            pickups.erase(pickups.begin() + i);
            break;
        }
    }
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
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


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    
	float cameraSpeed = 3.0f;
    glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));


	if (window->KeyHold(GLFW_KEY_W)) 
    {
		camera->MoveForward(deltaTime * cameraSpeed);
        player.changePos(deltaTime * cameraSpeed * dir);
        if (player.checkWalls(mazePattern,mazeSize, cellSize)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
            player.changePos(-deltaTime * cameraSpeed * dir);
        }
        if (player.outOfMaze(cellSize, mazeSize)) {
            gameOver = 1;
            win = 1;
        }
        checkForPickups();
	}
	if (window->KeyHold(GLFW_KEY_A))
	{
		camera->TranslateRight(-deltaTime * cameraSpeed);
        player.changePos(-deltaTime * cameraSpeed * camera->right);
		if (player.checkWalls(mazePattern,mazeSize, cellSize)) {
            camera->TranslateRight(deltaTime * cameraSpeed);
            player.changePos(deltaTime * cameraSpeed * camera->right);
        }
        if (player.outOfMaze(cellSize, mazeSize)) {
            gameOver = 1;
            win = 1;
        }
        checkForPickups();
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		camera->MoveForward(-deltaTime * cameraSpeed);
        player.changePos(-deltaTime * cameraSpeed * dir);
        if (player.checkWalls(mazePattern,mazeSize, cellSize)) {
            camera->MoveForward(deltaTime * cameraSpeed);
            player.changePos(deltaTime * cameraSpeed * dir);
        }
        if (player.outOfMaze(cellSize, mazeSize)) {
            gameOver = 1;
            win = 1;
        }
        checkForPickups();
	}
	if (window->KeyHold(GLFW_KEY_D))
	{
		camera->TranslateRight(deltaTime * cameraSpeed);
        player.changePos(deltaTime * cameraSpeed * camera->right);
        if (player.checkWalls(mazePattern,mazeSize, cellSize)) {
            camera->TranslateRight(-deltaTime * cameraSpeed);
            player.changePos(-deltaTime * cameraSpeed * camera->right);
        }
        if (player.outOfMaze(cellSize, mazeSize)) {
            gameOver = 1;
            win = 1;
        }
        checkForPickups();
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (cameraType == 0) {
            camera->TranslateForward(camera->distanceToTarget); 
        }
        else {
            camera->TranslateForward(-camera->distanceToTarget); 
        }
        cameraType = 1 - cameraType;
    }
    if (key == GLFW_KEY_ENTER) {
        if (gameOver) {
            window->Close();
        }
    }
    if (key == GLFW_KEY_0) {
        cout << angle << "\n";
    }
    if (key == GLFW_KEY_1) {
        cout << (int)(player.getPos().z/cellSize) << " " << (int)(player.getPos().x/cellSize) << "\n";
    }
    if (key == GLFW_KEY_2) {
        cout << player.getHitBox()<< "\n";
    }
    if (key == GLFW_KEY_3) {
        cout << availablePositions.size() << "\n";
    }
    if (key == GLFW_KEY_4) {
        health -= 0.05f;
    }
    if (key == GLFW_KEY_5) {
        health += 0.05f;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (cameraType == 0) {    
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }else{
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
        angle = atan2(camera->right.z, camera->right.x);
        player.setAng(-angle);
        if (player.checkWalls(mazePattern, mazeSize, cellSize)) {
            player.setAng(player.getPrevAng());
            if (cameraType == 0) {
                camera->RotateThirdPerson_OX(sensivityOX * deltaY);
                camera->RotateThirdPerson_OY(sensivityOY * deltaX);
            }
            else {
                camera->RotateFirstPerson_OX(sensivityOX * deltaY);
                camera->RotateFirstPerson_OY(sensivityOY * deltaX);
            }
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && cameraType == 1) {
        if (!player.getExplodingProj() && !player.getMultipleProj()) {
            Missile newProjectile(camera->position, camera->forward);
            projectiles.push_back(newProjectile);
        }
        else {
            if (player.getMultipleProj()) {
                for (int i = 0; i < 4; i++) {
                    glm::vec3 axis = glm::normalize(glm::vec3(randomUtils::randIntervalF(1.f, 5.f), randomUtils::randIntervalF(1.f, 5.f), randomUtils::randIntervalF(1.f, 5.f)));
                    glm::vec3 dir = glm::rotate(glm::mat4(1.f), randomUtils::randIntervalF(0.f, 0.2f), axis) * glm::vec4(camera->forward, 0);
                    Missile newProjectile(camera->position, dir);
                    if (player.getExplodingProj()) {
                        newProjectile.setExplodingStatus(true);
                    }
                    projectiles.push_back(newProjectile);
                }
            }
            if (player.getExplodingProj()) {
                Missile newProjectile(camera->position, camera->forward);
                newProjectile.setExplodingStatus(true);
                projectiles.push_back(newProjectile);
            }
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
