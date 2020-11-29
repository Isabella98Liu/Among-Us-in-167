#include "SkyBox.h"

SkyBox::SkyBox(vector<std::string> faceFiles)
{
	faces = faceFiles;

	loadMap();

	int scale = 500;
	// Generate a Cube for SkyBox
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-scale, scale, scale),
		glm::vec3(-scale, -scale, scale),
		glm::vec3(scale, -scale, scale),
		glm::vec3(scale, scale, scale),
		glm::vec3(-scale, scale, -scale),
		glm::vec3(-scale, -scale, -scale),
		glm::vec3(scale, -scale, -scale),
		glm::vec3(scale, scale, -scale)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
	};

	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SkyBox::~SkyBox()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &textureID);
}

void SkyBox::loadMap()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}

	// Make sure no bytes are padded
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Use clamp to edge to hide skybox edges
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Unbind 
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));

	// Bind the VAO
	glBindVertexArray(VAO);
	glActiveTexture(textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Disable front face culling
	glCullFace(GL_BACK);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}