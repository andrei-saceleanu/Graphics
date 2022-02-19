#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace m1;

#define PI 3.14159265


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
float randIntervalF(float a, float b)
{
    float f = (float)rand() / (float)(RAND_MAX);
    return (b - a) * f + a;
}

int randIntervalI(int a, int b)
{
    int x = rand() % (b - a + 1);
    return a + x;
}

Mesh* CreateCone(const std::string& name, float radius, glm::vec3 color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    GLfloat arg;

    vertices.emplace_back(glm::vec3(0, -1, 0), color);
    for (GLushort i = 0; i < 40; i++)
    {
        arg = 2 * 3.14f * i / 40;

        vertices.emplace_back(glm::vec3(radius * cos(arg), 0, radius * sin(arg)));
        indices.push_back(i);
    }
    indices.push_back(40);
    indices.push_back(1);
    Mesh* mesh = new Mesh(name);
    mesh->SetDrawMode(GL_TRIANGLE_FAN);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* CreateSphere(const std::string& name, float radius,int stackCount,int sectorCount)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;
    float xy, z, x, y, nx, ny, nz;
    float lengthInv = 1.f / radius;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        
        xy = radius * cosf(stackAngle);             
        z = radius * sinf(stackAngle);              

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             
            y = xy * sinf(sectorAngle);             

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            float s, t;
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.emplace_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(1), glm::vec3(nx, ny, nz),glm::vec2(s,t)));
        }
    }
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     
        k2 = k1 + sectorCount + 1;      

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

        }
    }
    Mesh* mesh = new Mesh(name);
    mesh->SetDrawMode(GL_TRIANGLES);
    mesh->InitFromData(vertices, indices);
    return mesh;
}



Tema3::Tema3()
{
    cellSize = 1.f;
    floorSize = 8;
    alpha = 0.f;
    colors = (glm::vec3**)malloc(floorSize * sizeof(glm::vec3*));
    for (int i = 0; i < floorSize; i++) {
        colors[i] = (glm::vec3*)malloc(floorSize * sizeof(glm::vec3));
        for (int j = 0; j < floorSize; j++) {
            colors[i][j] = glm::vec3(randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f));
        }
    }
    new_colors = (glm::vec3**)malloc(floorSize * sizeof(glm::vec3*));
    for (int i = 0; i < floorSize; i++) {
        new_colors[i] = (glm::vec3*)malloc(floorSize * sizeof(glm::vec3));
        for (int j = 0; j < floorSize; j++) {
            new_colors[i][j] = glm::vec3(randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f));
        }
    }
    for (int i = 0; i < 4; i++) {
        ceilLightPos[i] = glm::vec3(randIntervalI(1, floorSize - 1), 5.f, randIntervalI(1, floorSize - 1));
        ceilLightDir[i] = glm::vec3(0, -1, 0);
        currentCeilColor[i] = glm::vec3(randIntervalF(0, 1), randIntervalF(0, 1), randIntervalF(0, 1));
        nextCeilColor[i] = glm::vec3(randIntervalF(0, 1), randIntervalF(0, 1), randIntervalF(0, 1));
    }

    for (int i = 0; i < 5; i++) {
        jumping[i] = false;
    }
    cutoffAngle = 3.14159f/12;
    show = 0;
}


Tema3::~Tema3()
{
}



void Tema3::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = CreateCone("cone", 1.f, glm::vec3(1, 0, 0));
        AddMeshToList(mesh);
    }

    

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
        Shader *shader = new Shader("Tema3Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    mapTextures["discoBall"] = CreateRandomTexture(0, 16, 16);
    mapTextures["next_discoBall"] = CreateRandomTexture(1, 16, 16);



    {
        materialShininess = 2;
        materialKe = 1.2;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    for (int k = 0; k < 9; k++) {
        if (k < 5) { // dancers
            for (int i = 0; i < 7; i++) {
                glm::vec3 v = glm::vec3(randIntervalF(1, 7), 0.25f, randIntervalF(1, 7));
                bezier[k][i][0] = v;
            }
        }
        else {
            for (int i = 0; i < 7; i++) {
                glm::vec3 v = glm::vec3(randIntervalF(2, 6), 0.f, randIntervalF(2, 6));
                bezier[k][i][0] = v;
            }
        }
    }
    t = 0;
    sgn = 1;
    startTransitionTime = Engine::GetElapsedTime();
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    for (int k = 0; k < 9; k++) {
        for (int j = 1; j < 7; j++) {
            for (int i = j; i < 7; i++) {
                bezier[k][i][j] = (1 - t) * bezier[k][i - 1][j - 1] + t * bezier[k][i][j - 1];
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        float time = Engine::GetElapsedTime();
        if (follow.find(i)!=follow.end()) {
            if (time - follow[i].first <= 3.f) {
                ceilLightDir[i] = bezier[follow[i].second][6][6] - ceilLightPos[i];
            }
            else {
                ceilLightDir[i] = bezier[i + 5][6][6] - ceilLightPos[i];
                follow.erase(i);
            }
        }
        else {
            float p = randIntervalF(0.f, 1.f);
            if (p <= 0.9f) {
                ceilLightDir[i] = bezier[i + 5][6][6] - ceilLightPos[i];
            }
            else {
                int dancerIndex = randIntervalI(0, 5);
                ceilLightDir[i] = bezier[dancerIndex][6][6] - ceilLightPos[i];
                follow[i] = { Engine::GetElapsedTime(),dancerIndex };
            }
        }
    }
    t += 0.005*sgn;
    if (t >= 1 || t <= 0) {
        sgn = -sgn;
    }

    
}


void Tema3::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix;
    glm::vec2 v;

    
    if (alpha>=1.f) {
        for (int i = 0; i < floorSize; i++) {
            for (int j = 0; j < floorSize; j++) {
                colors[i][j] = new_colors[i][j];
                new_colors[i][j] = glm::vec3(randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f), randIntervalF(0.f, 1.f));
            }
        }
        for (int i = 0; i < 4; i++) {
            currentCeilColor[i] = nextCeilColor[i];
            nextCeilColor[i] = glm::vec3(randIntervalF(0, 1), randIntervalF(0, 1), randIntervalF(0, 1));
        }
        startTransitionTime = Engine::GetElapsedTime();
        
        mapTextures["discoBall"] = CreateRandomTexture(0, 16, 16);
        mapTextures["next_discoBall"] = CreateRandomTexture(1, 16, 16);

    }

    alpha = (Engine::GetElapsedTime() - startTransitionTime) / 3.f;


    glm::vec3 p = bezier[8][6][6];
    RenderFractal(p, 0.5f);
    
    //Render dancers
    for (int i = 0; i < 5; i++) {
        glm::vec3 pos = bezier[i][6][6];
        if (jumping[i] == false) {
            float p = randIntervalF(0.f, 1.f);
            if (p <= 0.01f) {
                jumping[i] = true;
                startJumpingTime[i] = Engine::GetElapsedTime();
            }
        }
        else {
            float t_offset = Engine::GetElapsedTime() - startJumpingTime[i];
            pos.y = 0.25f + 0.5f * t_offset - 0.5f * 0.5f * t_offset * t_offset;
            if (pos.y <= 0.2499f) {
                jumping[i] = false;
                pos.y = 0.25f;
            }
        }
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
        int x = pos.z / cellSize;
        int y = pos.x / cellSize;
        v = glm::vec2(x, y);
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);
    }

	//Render walls

	for (int j = 0; j < floorSize; j++) {

		//front wall
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * j, 0.f, cellSize * (-1.f)));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize, 5.f, cellSize));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
		v = glm::vec2(-1, j);
		RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);

		//left wall
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * (-1.f), 0.f, cellSize * j));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize, 5.f, cellSize));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
		v = glm::vec2(j, -1);
		RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);


		//right wall
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * floorSize, 0.f, cellSize * j));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize, 5.f, cellSize));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
		v = glm::vec2(j, floorSize);
		RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);
	}

    //Render disco ball
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize*(floorSize/2), 4.f, cellSize * (floorSize / 2)));
    RenderSimpleMesh(meshes["sphere"], shaders["Tema3Shader"], modelMatrix, glm::vec3(1), 3,glm::vec2(1),mapTextures["discoBall"], mapTextures["next_discoBall"]);

	// Render dance floor and ceiling

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * j, 0.f, cellSize * i));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize / 50, 1.f, cellSize / 50));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(25.f, 0.f, 25.f));
			RenderSimpleMesh(meshes["plane"], shaders["Tema3Shader"], modelMatrix, glm::mix(colors[i][j],new_colors[i][j],alpha));
		}
	}

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(cellSize * j, 5.f, cellSize * i));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(cellSize, 1.f, cellSize));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
            v = glm::vec2(i, j);
            RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);

        }
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for (int i = 0; i < 4; i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, ceilLightPos[i]);
        glm::vec3 d(ceilLightDir[i]);
        float ang1 = acos(d.y / length(d));
        float ang2 = atan2(d.z, d.x);
        modelMatrix = glm::rotate(modelMatrix, ang2, glm::vec3(0, -1, 0));
        modelMatrix = glm::rotate(modelMatrix, 3.14159f / 2 - ang1, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, 3.14159f / 2, glm::vec3(0, 0, 1));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -10, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(cutoffAngle), -1.f, tan(cutoffAngle)) * 10.f);
        RenderSimpleMesh(meshes["cone"], shaders["Tema3Shader"], modelMatrix, ceilLightColor[i], 2);
    }

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    

}


void Tema3::FrameEnd()
{
 
}

bool inside(int x, int y, int sideSize) {
    return (x >= 0) && (x < sideSize) && (y >= 0) && (y < sideSize);
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color,int type,glm::vec2 pos,Texture2D* t,Texture2D* t2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    int alphaLoc = glGetUniformLocation(shader->program, "alpha");
    glUniform1f(alphaLoc, alpha);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int locType = glGetUniformLocation(shader->program, "type");
    glUniform1i(locType, type);

    int locShow = glGetUniformLocation(shader->program, "show");
    glUniform1i(locShow, show);


    glm::vec3 pointLightPos[9];
    glm::vec3 pointLightColor[9];
    int k = 0;
    int x = pos.x;
    int y = pos.y;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            pointLightPos[k] = glm::vec3(cellSize * (pos.y + j) + cellSize / 2, 0.f, cellSize * (pos.x + i) + cellSize / 2);
            if (inside(x + i, y + j, floorSize))
                pointLightColor[k] = glm::mix(colors[x + i][y + j],new_colors[x+i][y+j],alpha);
            else
                pointLightColor[k] = glm::vec3(0);
            k++;
        }
    }
    int locationLightPos = glGetUniformLocation(shader->program, "pointLightPos");
    glUniform3fv(locationLightPos, 9, glm::value_ptr(pointLightPos[0]));


    int locationLightColor = glGetUniformLocation(shader->program, "pointLightColor");
    glUniform3fv(locationLightColor, 9, glm::value_ptr(pointLightColor[0]));

    int locCeilPos = glGetUniformLocation(shader->program, "ceilPos");
    glUniform3fv(locCeilPos, 4, glm::value_ptr(ceilLightPos[0]));

    int locCeilDir = glGetUniformLocation(shader->program, "ceilDir");
    glUniform3fv(locCeilDir, 4, glm::value_ptr(ceilLightDir[0]));

    int locCutoff = glGetUniformLocation(shader->program, "cutoffAngle");
    glUniform1f(locCutoff, cutoffAngle);
    
    for (int i = 0; i < 4; i++) {
        ceilLightColor[i] = glm::mix(currentCeilColor[i], nextCeilColor[i], alpha);
    }
    int locationCeilCol = glGetUniformLocation(shader->program, "ceilColor");
    glUniform3fv(locationCeilCol, 4, glm::value_ptr(ceilLightColor[0]));

    if (t) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (t2) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, t2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    int locDisco = glGetUniformLocation(shader->program, "discoPos");
    glUniform3fv(locDisco, 1, glm::value_ptr(glm::vec3(cellSize * (floorSize / 2), 4.f, cellSize * (floorSize / 2))));

    int locTime = glGetUniformLocation(shader->program, "time");
    glUniform1f(locTime,Engine::GetElapsedTime());


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int id,unsigned int width, unsigned int height)
{
    GLuint textureID = id;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    for (size_t i = 0; i < size; ++i)
    {
        data[i] = rand() % (UINT8_MAX + 1);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    //CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::RenderFractal(glm::vec3 offset, float scale)
{
    if (scale <= 0.06f) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, offset);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f));
        int x = offset.z / cellSize;
        int y = offset.x / cellSize;
        glm::vec2 v = glm::vec2(x, y);
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0.f), 1, v, mapTextures["discoBall"], mapTextures["next_discoBall"]);
    }
    else {
        for (int k = 0; k < 3; k++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((k == 1 && i == 1) || (k == 1 && j == 1)) {
                        continue;
                    }
                    else if ((k == 2 && i == 1 && j == 1) || (k == 0 && i == 1 && j == 1)) {
                        continue;
                    }
                    else {
                        glm::vec3 newOffset;
                        newOffset.x = offset.x + scale * j;
                        newOffset.y = offset.y + scale * k;
                        newOffset.z = offset.z + scale * i;
                        RenderFractal(newOffset, scale / 3.f);
                    }
                }
            }
        }
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        show = (show + 1) % 4;
    }
    
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
