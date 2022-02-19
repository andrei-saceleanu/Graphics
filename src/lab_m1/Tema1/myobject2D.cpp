#include "myobject2D.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* myobject2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* myobject2D::CreateCircle(const std::string& name, float radius, glm::vec3 color, bool fill)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    GLfloat arg;

    vertices.emplace_back(glm::vec3(0, 0, 0), color);
    for (GLushort i = 0; i < 40; i++)
    {
        arg = 2 * 3.14f * i / 40;

        vertices.emplace_back(glm::vec3(radius*cos(arg), radius*sin(arg), 0), color);
        indices.push_back(i);
    }
    indices.push_back(40);
    indices.push_back(1);
    Mesh* mesh = new Mesh(name);
    mesh->SetDrawMode(GL_TRIANGLE_FAN);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* myobject2D::CreateDiscus(const std::string& name, float radius, glm::vec3 color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    GLfloat arg;

    for (GLushort i = 0; i <= 500; i++)
    {
        arg = 2 * 3.14f * i / 500;
        vertices.emplace_back(VertexFormat(glm::vec3(radius * cos(arg), radius * sin(arg), 0), color));
        indices.push_back(i);
    }

    Mesh* mesh = new Mesh(name);
    mesh->SetDrawMode(GL_LINE_LOOP);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* myobject2D::CreateDigit(char digit, glm::vec3 color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    vertices.emplace_back(VertexFormat(glm::vec3(0, 0, 0), color));
    vertices.emplace_back(VertexFormat(glm::vec3(0, 1, 0), color));
    vertices.emplace_back(VertexFormat(glm::vec3(0, 2, 0), color));
    vertices.emplace_back(VertexFormat(glm::vec3(1, 2, 0), color));
    vertices.emplace_back(VertexFormat(glm::vec3(1, 1, 0), color));
    vertices.emplace_back(VertexFormat(glm::vec3(1, 0, 0), color));
    string name(1, digit);
    Mesh* mesh = new Mesh(name);
    switch (digit) {
    case '0':
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(0);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '1':
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '2':
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(5);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '3':
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(1);
        indices.push_back(4);
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(4);
        indices.push_back(5);
        mesh->SetDrawMode(GL_LINES);
        break;
    case '4':
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(1);
        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(4);
        indices.push_back(5);
        mesh->SetDrawMode(GL_LINES);
        break;
    case '5':
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '6':
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(1);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '7':
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    case '8':
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(5);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(4);
        mesh->SetDrawMode(GL_LINES);
        break;
    case '9':
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(4);
        mesh->SetDrawMode(GL_LINE_STRIP);
        break;
    default:
        break;
    }
    
    mesh->InitFromData(vertices, indices);
    return mesh;
}
