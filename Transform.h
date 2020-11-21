#pragma once
#include "Node.h"

#include <vector>

using namespace std;

class Transform :
    public Node
{
private:
    glm::mat4 MT = glm::mat4(1);   // transform matrix
    std::vector<Node*> childs;  // list of child nodes

public:
    Transform();
    ~Transform();

    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);

    void addChild(Node* child);
    void addChilds(std::vector<Transform*> child);
    void setTransform(glm::mat4 M) { MT = M; };

    int getSize() { return childs.size(); }
};

