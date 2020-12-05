#pragma once
#include "Node.h"
#include "Geometry.h"

#include <string>
#include <vector>

class Character : public Node
{
private:
	std::vector<Geometry*> frames;
	Geometry* currentFrame;

	GLuint textureID = 0;
	GLuint shaderID = 0;
	Material* material = NULL;

public:
	Character(std::vector<std::string> frameFiles, int mode);
	~Character();

	void draw(glm::mat4 C);
	void update(glm::mat4 C);

	void useTexture(GLuint id);
	void useShader(GLuint id);
	void useMaterial(Material* mat);

};

