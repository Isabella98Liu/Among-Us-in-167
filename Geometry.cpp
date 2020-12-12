#include "Geometry.h"

/* 
	MODE 1 - f 1//1//1 2//2//2 3//3//3
	MODE 2 - f 1/1/1 2/2/2 3/3/3
	TODO: load uvs*/
Geometry::Geometry(std::string objFilename, int mode)
{
	loadMode = mode;

	if (loadMode == 1)
		loadMode1(objFilename);
	else if (loadMode == 2)
		loadMode2(objFilename);

	// Normalize the object to fit in the screen
	normalize();
	
	// Generate a Vertex Array and Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind NBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 1 to pass normal data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	if(loadMode == 1)
	{
		// Generate EBO, bind the EBO to the bound VAO, and send the index data
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}	

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::loadMode1(std::string objFilename)
{
	// Read obj data from file
	std::ifstream objFile(objFilename);
	if (objFile.is_open())
	{
		std::string line;
		while (std::getline(objFile, line))
		{
			std::stringstream ss;
			ss << line;
			//Read the first word of the line
			std::string label;
			ss >> label;
			if (label == "v")
			{
				glm::vec3 vertex;
				ss >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if (label == "vn")
			{
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				normal = glm::normalize(normal);
				normals.push_back(normal);
			}
			else if (label == "f")
			{
				glm::uvec3 indice;
				ss >> indice.x;
				ss.ignore(10, ' ');
				ss >> indice.y;
				ss.ignore(10, ' ');
				ss >> indice.z;
				ss.ignore(10, ' ');
				indice -= 1;
				indices.push_back(indice);
			}
		}
	}
	else
		std::cerr << "Can't open the file " << objFilename << std::endl;
	objFile.close();
}

/* Model load tutorial: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/*/
void Geometry::loadMode2(std::string objFilename)
{
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::uvec3> vertexIndices, uvIndices, normalIndices;

	FILE* file = fopen(objFilename.c_str(), "r");
	if (file == NULL) 
	{
		printf("Can not open the file : %s\n", objFilename.c_str());
		return;
	}
	char line[128];
	int res = fscanf(file, "%s", line);
	while (res != EOF)
	{
		if (strcmp(line, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(line, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("File can not be parsed by loadMode2\n");
				return;
			}
			vertexIndices.push_back(glm::uvec3(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
			uvIndices.push_back(glm::uvec3(uvIndex[0], uvIndex[1], uvIndex[2]));
			normalIndices.push_back(glm::uvec3(normalIndex[0], normalIndex[1], normalIndex[2]));
		}
		res = fscanf(file, "%s", line);
	}
	/* processing data: when produce the uvs and normals can not simply duplicate the size of vertices and loop over
	because the number of uvs and normals could be larger than the size vertices
	*/
	for (int i = 0; i < vertexIndices.size(); i++)
	{
		int vertex_index = vertexIndices[i].x - 1;
		int uv_index = uvIndices[i].x - 1;
		int normal_index = normalIndices[i].x - 1;
		vertices.push_back(temp_vertices[vertex_index]);
		uvs.push_back(temp_uvs[uv_index]);
		normals.push_back(temp_normals[normal_index]);

		vertex_index = vertexIndices[i].y - 1;
		uv_index = uvIndices[i].y - 1;
		normal_index = normalIndices[i].y - 1;
		vertices.push_back(temp_vertices[vertex_index]);
		uvs.push_back(temp_uvs[uv_index]);
		normals.push_back(temp_normals[normal_index]);

		vertex_index = vertexIndices[i].z - 1;
		uv_index = uvIndices[i].z - 1;
		normal_index = normalIndices[i].z - 1;
		vertices.push_back(temp_vertices[vertex_index]);
		uvs.push_back(temp_uvs[uv_index]);
		normals.push_back(temp_normals[normal_index]);
	}
}

Geometry::~Geometry()
{
	// Delete the VBO/VEO and VAO
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Geometry::draw(glm::mat4 C)
{
	// Send material information to shader
	if (material != NULL)
		material->sendMatToShader(shaderID);

	// Actiavte the shader program 
	glUseProgram(shaderID);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(C * model));

	// Bind the VAO
	glBindVertexArray(VAO);

	// If a texture was assigned, pass texture to shader
	if (textureID != 0)
	{
		glActiveTexture(textureID);
		if(envMapping)
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		else
			glBindTexture(GL_TEXTURE_2D, textureID);
	}

	// Draw the points using triangles, indexed with the EBO
	if (loadMode == 1)
	{
		glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (loadMode == 2)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	
	//Unbind
	if (envMapping)
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	else
		glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Geometry::useTexture(GLuint id)
{

	textureID = id;

	if (envMapping) return;
	// generate buffer for TBO and bind it to VAO
	glGenBuffers(1, &TBO);

	glBindVertexArray(VAO);

	// Bind TBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/* Update() is used for updating world coordinates, for local coordinates, we should update model instead*/

void Geometry::update(glm::mat4 C)
{
	model = C * model;
}

void Geometry::rotate(GLfloat deg, glm::vec3 axis)
{
	model = glm::rotate(deg, axis) * model;
}

void Geometry::normalize()
{
	glm::vec3 points_max = vertices[0];
	glm::vec3 points_min = vertices[0];
	for (unsigned int i = 1; i < vertices.size(); i++) {
		if (vertices[i].x < points_min.x)
			points_min.x = vertices[i].x;
		else if (vertices[i].x > points_max.x)
			points_max.x = vertices[i].x;

		if (vertices[i].y < points_min.y)
			points_min.y = vertices[i].y;
		else if (vertices[i].y > points_max.y)
			points_max.y = vertices[i].y;

		if (vertices[i].z < points_min.z)
			points_min.z = vertices[i].z;
		else if (vertices[i].z > points_max.z)
			points_max.z = vertices[i].z;
	}
	glm::vec3 centroid = glm::vec3((points_min.x + points_max.x) / 2.0f, (points_min.y + points_max.y) / 2.0f, (points_min.z + points_max.z) / 2.0f);
	
	// Get the max distance of any point to the centroid
	GLfloat distance_max = 0.0f;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		GLfloat distance = glm::length(vertices[i] - centroid);
		if (distance > distance_max)
			distance_max = distance;
	}
	GLfloat scale_factor = 10.0f / distance_max;
	
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Take the identity matrix and apply translation to it
	model = glm::translate(-centroid) * model;
	model = glm::scale(glm::vec3(scale_factor, scale_factor, scale_factor)) * model;
}

void Geometry::resize(double offset)
{
	model = glm::scale(glm::vec3(1.0f + (float)offset * 0.02f)) * model;
}

void Geometry::rescale(glm::vec3 scale)
{
	model = glm::scale(scale) * model;
}

void Geometry::translate(glm::vec3 translation)
{
	model = glm::translate(translation) * model;
}