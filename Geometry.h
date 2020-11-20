#pragma once

#include "Node.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Geometry : public Node
{
private:
    GLuint VAO;
    GLuint VBO, EBO, NBO;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;

    glm::mat4 model = glm::mat4(1);

public:
    Geometry(std::string objFilename);
    ~Geometry();

    void draw(GLuint shaderProgram, glm::mat4 C, glm::mat4 projection);
    void update(glm::mat4 C);

    void spin(float deg);
    void normalize();
    void resize(double offset);
    void rotate(glm::vec3 start, glm::vec3 end);
    void translate(glm::vec3 translation);

    glm::mat4 getModel() { return model; }
    void setModel(glm::mat4 m) { model = m; }

};
