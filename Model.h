#pragma once

#include "Object.h"

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Model : public Object
{
private:
    GLuint VAO;
    GLuint VBO, EBO;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;

public:
    Model(std::string objFilename);
    ~Model();

    void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    void update();

    void spin(float deg);
    void normalize();
    void resize(double offset);
    void rotate(glm::vec3 start, glm::vec3 end);
};

