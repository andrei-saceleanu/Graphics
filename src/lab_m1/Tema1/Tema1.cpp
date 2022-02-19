#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/myobject2D.h"
#include "lab_m1/Tema1/collision.h"
#include <lab_m1/Tema1/Player.h>
#include <lab_m1/Tema1/Obstacle.h>
#include <lab_m1/Tema1/Projectile.h>


using namespace std;
using namespace m1;



Tema1::Tema1()
{
    squareSide = 50;
    sceneSize = 1000;
    speed = 300;
    srand(time(NULL));
    fireRate = 10.f;
    globalTime = 0.f;
    enemyTimeUnit = 10.f;
    pickupTimeUnit = 20.f;
    defenseTimeUnit = 10.f;
    newEnemyCount = 5;
    initialEnemyCount = 10;
    maxEnemyIndex = 0;
    maxDistanceProjectile = 300.f;
    score = 0;
    gameOver = false;
    cntObstacles = 6;
    cellSize = 50;
    int dim = (sceneSize / cellSize);
    occupied = (unsigned char**)malloc(dim * sizeof(unsigned char*));
    for (int i = 0; i < dim; i++) {
        occupied[i] = (unsigned char*)calloc(dim, sizeof(unsigned char));
    }

    modelMatrix = glm::mat3(1);
    visMatrix = glm::mat3(1);
    healthbar = new HealthBar(350, 325);
}


Tema1::~Tema1()
{
}




void Tema1::putObstaclesInScene() {
    glm::vec2 pos, dim;
    glm::mat3 modelMatrix;
    Obstacle* o;
    
    int currentCount = 0;
    int collisionFlag = 0;
    vector<Obstacle> v;
    float playerRadius = player.GetBodyRadius();
    while (currentCount < cntObstacles) {
        pos = glm::vec2(randUtils::randInterval(1.5f*playerRadius, sceneSize-playerRadius-1), randUtils::randInterval(1.5f*playerRadius, sceneSize - playerRadius - 1));
        dim = glm::vec2(randUtils::randInterval(100,200), randUtils::randInterval(100,200));
        collisionFlag = 0;
        if (!collision::rectWithinBoundary(pos, dim, sceneSize, sceneSize)) {
            continue;
        }
        for (int i = 0; i < currentCount; i++) {
            if (collision::rectRectCollision(pos, dim, obstacles[i].GetPosition(), obstacles[i].GetDimensions())) {
                collisionFlag = 1;
                break;
            }
        }
        if (collisionFlag) {
            continue;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(pos.x, pos.y);
        modelMatrix *= transform2D::Scale(dim.x, dim.y);
        o = new Obstacle(modelMatrix, pos, dim);
        v.push_back(*o);
        if (player.checkCollisionObstacle(v)) {
            v.clear();
            continue;
        }
        obstacles.push_back(*o);
        removeObstacleCells(*o);
        currentCount += 1;
    }
}

void Tema1::spawnEnemies()
{
    for (int i = 0; i < initialEnemyCount; i++) {
        Enemy newEnemy(sceneSize, sceneSize, randUtils::randInterval(1.f, 1.25f), i);
        enemies.push_back(newEnemy);
    }
    maxEnemyIndex = initialEnemyCount;
}

vector<Mesh*> fromNumber(int nr) {
    vector<Mesh*> v;
    if (nr == 0) {
        v.push_back(myobject2D::CreateDigit('0', glm::vec3(1, 1, 1)));
        return v;
    }
    int digit;
    while (nr) {
        digit = nr % 10;
        char c = (char)('0' + digit);
        Mesh* m = myobject2D::CreateDigit(c, glm::vec3(1, 1, 1));
        v.insert(v.begin(), m);
        nr = nr / 10;
    }
    return v;
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 400;   // logic width
    logicSpace.height = 400;  // logic height

    logicSpaceMinimap.x = 0;       
    logicSpaceMinimap.y = 0;       
    logicSpaceMinimap.width = 700;    
    logicSpaceMinimap.height = 700;

    glm::vec3 corner = glm::vec3(0, 0, 0);
    globalTime = 1.f/fireRate;

    

    Mesh* square1 = myobject2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    Mesh* square2 = myobject2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
    Mesh* obstacle = myobject2D::CreateSquare("obstacle", corner, 1, glm::vec3(1, 0, 0), true);
    Mesh* projectile = myobject2D::CreateCircle("projectile", 1, glm::vec3(0.2345f, 0.34f, 0.1f), true);
    Mesh* boundary = myobject2D::CreateSquare("boundary", corner, squareSide, glm::vec3(0.5f, 0.5f, 0.5f), true);

    
    AddMeshToList(square1);
    AddMeshToList(square2);
    AddMeshToList(projectile);
    AddMeshToList(boundary);
    AddMeshToList(obstacle);

    
    putObstaclesInScene();
    spawnEnemies();
}


void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    if (!gameOver) {
        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        viewSpaceMinimap = ViewportSpace(resolution.x - resolution.x / 7, 0, resolution.x / 7, resolution.y / 7);
        SetViewportArea(viewSpace, glm::vec3(0), true);

        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
        globalTime -= deltaTimeSeconds;
        enemyTimeUnit -= deltaTimeSeconds;
        pickupTimeUnit -= deltaTimeSeconds;
        if (player.GetHitBoxStatus()) {
            defenseTimeUnit -= deltaTimeSeconds;
            if (defenseTimeUnit <= 0) {
                player.SetHitBoxStatus(false);
                defenseTimeUnit = 10.f;
            }
        }
        if (enemyTimeUnit <= 0) {
            for (int i = 0; i < newEnemyCount; i++) {
                Enemy newEnemy(sceneSize, sceneSize, randUtils::randInterval(1.f, 1.25f), maxEnemyIndex);
                enemies.push_back(newEnemy);
                maxEnemyIndex += 1;
            }
            enemyTimeUnit = 10.f;
        }
        
        if (pickupTimeUnit <= 0) {
            int flag;
            Pickup* p;
            while (true) {
                float choose = (float)rand() / (float)RAND_MAX;
                if (choose <= 0.5f)
                    p = new HealthPickup(sceneSize, sceneSize);
                else
                    p = new DefensePickup(sceneSize, sceneSize);
                int x = p->GetPos().x / cellSize;
                int y = p->GetPos().y / cellSize;
                if (occupied[x][y] == 0)
                    break;
            }
            pickups.push_back(p);
            cout << "pickup occurence\n";
            pickupTimeUnit = 30.f;
        }

        v = fromNumber(score);
        MoveProjectiles(deltaTimeSeconds);
        MoveEnemies();
        DrawScene(visMatrix);
        glEnable(GL_DEPTH_TEST);
        RenderHealthbar(visMatrix);
        glDisable(GL_DEPTH_TEST);
        RenderScore(visMatrix);
        
        
        SetViewportArea(viewSpaceMinimap, glm::vec3(0), true);
        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpaceMinimap, viewSpaceMinimap);
        DrawScene(visMatrix);
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::DrawScene(glm::mat3 visMatrix) {
    RenderPlayer(visMatrix);
    RenderBoundary(visMatrix);
    RenderObstacles(visMatrix);
    RenderProjectiles(visMatrix);
    RenderEnemies(visMatrix);
    RenderPickups(visMatrix);

}


void Tema1::RenderPlayer(glm::mat3 visMatrix) {
    glm::mat3 bodyMat = player.GetBodyMatrix();
    glm::mat3 frontMat = player.GetFrontMatrix();
    glm::mat3 leftThruster = player.GetThrusterMatrix(0);
    glm::mat3 rightThruster = player.GetThrusterMatrix(1);
    bodyMat = visMatrix * bodyMat;
    frontMat = visMatrix * frontMat;
    leftThruster = visMatrix * leftThruster;
    rightThruster = visMatrix * rightThruster;

    if(player.GetHitBoxStatus())
        RenderMesh2D(player.GetHitBoxMesh(), shaders["VertexColor"], visMatrix * player.GetHitBoxMatrix());
    RenderMesh2D(player.GetLeftThrusterMesh(), shaders["VertexColor"], leftThruster);
    RenderMesh2D(player.GetRightThrusterMesh(), shaders["VertexColor"], rightThruster);
    RenderMesh2D(player.GetFrontMesh(), shaders["VertexColor"], frontMat);
    RenderMesh2D(player.GetBodyMesh(), shaders["VertexColor"], bodyMat);
    
}

void Tema1::RenderBoundary(glm::mat3 visMatrix)
{
    glm::mat3 top, bottom, left, right;
    
    top = glm::mat3(1);
    top = visMatrix * top;
    top *= transform2D::Translate(0, sceneSize);
    top *= transform2D::Scale(sceneSize/squareSide, 10);

    bottom = glm::mat3(1);
    bottom = visMatrix * bottom;
    bottom *= transform2D::Scale(10, 10);
    bottom *= transform2D::Translate(0, -squareSide);
    bottom *= transform2D::Scale(sceneSize/squareSide, 1);

    left = glm::mat3(1);
    left = visMatrix * left;
    left *= transform2D::Translate(-10*squareSide, -sceneSize/4);
    left *= transform2D::Scale(10, 10*sceneSize/squareSide);

    right = glm::mat3(1);
    right = visMatrix * right;
    right *= transform2D::Translate(sceneSize, 0);
    right *= transform2D::Scale(20, 10);
    right *= transform2D::Scale(1, sceneSize/squareSide);

    RenderMesh2D(meshes["boundary"], shaders["VertexColor"], top);
    RenderMesh2D(meshes["boundary"], shaders["VertexColor"], bottom);
    RenderMesh2D(meshes["boundary"], shaders["VertexColor"], left);
    RenderMesh2D(meshes["boundary"], shaders["VertexColor"], right);
}

void Tema1::RenderObstacles(glm::mat3 visMatrix)
{
    for (Obstacle& o : obstacles) {
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], visMatrix * o.GetModelMatrix());
    }
}

void Tema1::RenderProjectiles(glm::mat3 visMatrix)
{
    for (Projectile& p : projectiles) {
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], visMatrix * p.GetModelMatrix());
    }
}


void Tema1::MoveProjectiles(float deltaTime)
{
    vector<Projectile> v;
    for (Projectile& p : projectiles) {
        p.UpdatePos(deltaTime);
        if (p.GetDistanceTravelled()<=maxDistanceProjectile && p.checkObstacles(obstacles) && collision::circleWithinBoundary(p.GetPos(),p.GetRadius(),sceneSize,sceneSize)) {
            v.push_back(p);
        }
    }
    projectiles.clear();
    projectiles.insert(projectiles.begin(), v.begin(), v.end());
}

void Tema1::RenderHealthbar(glm::mat3 visMatrix)
{
    RenderMesh2D(healthbar->GetFrameMesh(), shaders["VertexColor"], visMatrix * healthbar->GetFrameMatrix());
    RenderMesh2D(healthbar->GetBodyMesh(), shaders["VertexColor"], visMatrix * healthbar->GetBodyMatrix());
}

void Tema1::RenderEnemies(glm::mat3 visMatrix)
{
    for (Enemy& e : enemies) {
        RenderMesh2D(e.GetMesh(), shaders["VertexColor"], visMatrix * e.GetMainMatrix());
        RenderMesh2D(e.GetMesh(), shaders["VertexColor"], visMatrix * e.GetSecMatrix(0));
        RenderMesh2D(e.GetMesh(), shaders["VertexColor"], visMatrix * e.GetSecMatrix(1));
    }
    
}

void Tema1::RenderPickups(glm::mat3 visMatrix)
{
    for (int i = 0; i < pickups.size(); i++) {
        RenderMesh2D(pickups[i]->GetMesh(), shaders["VertexColor"], visMatrix * pickups[i]->GetModelMatrix());
    }
}

void Tema1::RenderScore(glm::mat3 visMatrix)
{
    for (int i = 0; i < v.size(); i++) {
        glm::mat3 m = glm::mat3(1);
        m *= transform2D::Translate(logicSpace.x + i * 25, logicSpace.y + logicSpace.height - 80);
        m *= transform2D::Scale(20, 20);
        RenderMesh2D(v[i], shaders["VertexColor"], visMatrix * m);
    }
}

void Tema1::MoveEnemies()
{
    vector<Enemy> v;
    bool c1, c2;
    for (Enemy& e : enemies) {
        e.SeekPlayer(player.GetPos(),enemies,projectiles);
        if(e.CheckBoundary(sceneSize, sceneSize))
            e.UpdatePos();
        c1 = false;
        if(!player.GetHitBoxStatus())
            c1 = player.checkCollisionEnemy(e);
        c2 = e.checkProjectiles(projectiles);
        if (!c1 && !c2) {
            if ((player.GetHitBoxStatus()==true&&!e.checkHitBox(player.GetHitBox())) || (player.GetHitBoxStatus()==false)) {
                v.push_back(e);
            }
        }
        else {
            if (c1) {
                healthbar->changeScale(-5);
                if (healthbar->GetScale() <= 0) {
                    cout << "Game over! Final score: "<<score<<"\n";
                    gameOver = true;
                }
            }
            if (c2) {
                score++;
                cout << score << "\n";
            }
        }
        
    }
    enemies.clear();
    enemies.insert(enemies.begin(), v.begin(), v.end());
}

void Tema1::removeObstacleCells(Obstacle o)
{
    glm::vec2 pos = o.GetPosition();
    glm::vec2 dim = o.GetDimensions();
    int x1 = pos.x/cellSize;
    int y1 = pos.y / cellSize;
    int x2 = (pos.x + dim.x) / cellSize;
    int y2 = (pos.y + dim.y) / cellSize;
    for (int i = x1; i <= x2; i++) {
        for (int j = y1; j <= y2; j++) {
            occupied[i][j] = 1;
        }
    }
}


glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(5);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::checkForPickups()
{
    bool b1, b2, b3, b4;
    vector<Pickup*> v;
    for (int i = 0; i < pickups.size(); i++) {
        b1 = collision::circleCircleCollision(player.GetPos(), player.GetBodyRadius(), pickups[i]->GetPos(), pickups[i]->GetRadius());
        b2 = collision::circleCircleCollision(player.GetFrontPos(), player.GetFrontRadius(), pickups[i]->GetPos(), pickups[i]->GetRadius());
        b3 = collision::circleCircleCollision(player.GetThrusterPos(0), player.GetThrusterRadius(), pickups[i]->GetPos(), pickups[i]->GetRadius());
        b4 = collision::circleCircleCollision(player.GetThrusterPos(1), player.GetThrusterRadius(), pickups[i]->GetPos(), pickups[i]->GetRadius());
        if (b1 || b2 || b3 || b4) {
            pickups[i]->applyAction(healthbar,player);
        }
        else {
            v.push_back(pickups[i]);
        }
    }
    pickups.clear();
    pickups.insert(pickups.begin(), v.begin(), v.end());
}



void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W))
    {
        logicSpace.y += speed*deltaTime;
        logicSpaceMinimap.y += speed * deltaTime;
        player.changeY(speed * deltaTime);
        healthbar->changeOffsetY(speed * deltaTime);
        if (!player.checkBoundary(sceneSize,sceneSize)||player.checkCollisionObstacle(obstacles)) {
            logicSpace.y -= speed * deltaTime;
            logicSpaceMinimap.y -= speed * deltaTime;
            player.changeY(-speed * deltaTime);
            healthbar->changeOffsetY(-speed * deltaTime);
        }
        checkForPickups();
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        logicSpace.y -= speed * deltaTime;
        logicSpaceMinimap.y -= speed * deltaTime;
        player.changeY(-speed * deltaTime);
        healthbar->changeOffsetY(-speed * deltaTime);
        if (!player.checkBoundary(sceneSize,sceneSize) || player.checkCollisionObstacle(obstacles)) {
            logicSpace.y += speed * deltaTime;
            logicSpaceMinimap.y += speed * deltaTime;
            player.changeY(speed * deltaTime);
            healthbar->changeOffsetY(speed * deltaTime);
        }
        checkForPickups();
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        logicSpace.x -= speed * deltaTime;
        logicSpaceMinimap.x -= speed * deltaTime;
        player.changeX(-speed * deltaTime);
        healthbar->changeOffsetX(-speed * deltaTime);
        if (!player.checkBoundary(sceneSize,sceneSize) || player.checkCollisionObstacle(obstacles)) {
            logicSpace.x += speed * deltaTime;
            logicSpaceMinimap.x += speed * deltaTime;
            player.changeX(speed * deltaTime);
            healthbar->changeOffsetX(speed * deltaTime);
        }
        checkForPickups();
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        logicSpace.x += speed * deltaTime;
        logicSpaceMinimap.x += speed * deltaTime;
        player.changeX(speed * deltaTime);
        healthbar->changeOffsetX(speed * deltaTime);
        if (!player.checkBoundary(sceneSize,sceneSize) || player.checkCollisionObstacle(obstacles)) {
            logicSpace.x -= speed * deltaTime;
            logicSpaceMinimap.x -= speed * deltaTime;
            player.changeX(-speed * deltaTime);
            healthbar->changeOffsetX(-speed * deltaTime);
        }
        checkForPickups();
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_1) {
        healthbar->changeScale(10);
    }
    if (key == GLFW_KEY_0) {
        healthbar->changeScale(-10);
    }
    if (key == GLFW_KEY_2) {
        cout << score << "\n";
    }
    if (key == GLFW_KEY_SPACE) {
        if (gameOver) {
            window->Close();
        }
    }

}


void Tema1::OnKeyRelease(int key, int mods)
{
}



void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 r = window->GetResolution();
    float ang = -atan2(mouseY - r.y/2, mouseX - r.x/2);
    
    if(player.checkAngleAtBoundary(ang,sceneSize,sceneSize)&&!player.checkCollisionAngleObstacle(obstacles,ang))
        player.SetAngle(ang);
    
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        if (globalTime <= 0) {
            Projectile p(player.GetFrontPos(), player.GetDir(), 300);
            projectiles.push_back(p);
            globalTime = 1.f / fireRate;
        }
    }
        
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
