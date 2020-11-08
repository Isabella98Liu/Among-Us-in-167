#pragma once

#include "Object.h"
#include "Material.h"

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
    GLuint VBO, EBO, NBO;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;

    //---
    glm::vec3 lightColor, lightPos;
    Material *material;
    GLboolean normalShadding;

public:
    Model(std::string objFilename);
    ~Model();

    void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    void update();

    void spin(float deg);
    void normalize();
    void resize(double offset);
    void rotate(glm::vec3 start, glm::vec3 end);
    void translate(glm::vec3 translation);

    void setMaterial(glm::vec3 ambientFactor, glm::vec3 diffuseFactor, glm::vec3 specularFactor, GLfloat shine, glm::vec3 color);
    void setNormalShadding();
};

