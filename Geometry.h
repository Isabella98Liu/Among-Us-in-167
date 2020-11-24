#pragma once

#include "Node.h"
#include "Material.h"

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
    GLuint VBO, EBO, NBO, TBO;  // NBO - normals, TBO - uvs

    int loadMode;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::uvec3> indices;

    glm::mat4 model = glm::mat4(1);

    GLuint textureID = 0;
    GLuint shaderID = 0;
    Material* material = NULL;



public:
    Geometry(std::string objFilename, int mode);
    ~Geometry();

    void loadMode1(std::string objFilename);
    void loadMode2(std::string objFilename);

    void draw(glm::mat4 C);
    void update(glm::mat4 C);

    void useTexture(GLuint id);
    void useShader(GLuint id) { shaderID = id; }
    void useMaterial(Material* mat) { material = mat; }

    void spin(float deg);
    void normalize();
    void resize(double offset);
    void rescale(glm::vec3 scale);
    void rotate(glm::vec3 start, glm::vec3 end);
    void translate(glm::vec3 translation);

    glm::mat4 getModel() { return model; }
    void setModel(glm::mat4 m) { model = m; }
};

