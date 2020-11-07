#include "Model.h"

Model::Model(std::string objFilename)
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
			if(label == "v")
			{
				glm::vec3 vertex;
				ss >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if(label == "vn")
			{
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (label == "f")
			{
				glm::ivec3 indice;
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

	// Normalize the object to fit in the screen
	normalize();
	
	// Set the color
	color = glm::vec3(0, 255, 222);

	// Generate a Vertex Array and Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
	
	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model()
{
	// Delete the VBO/VEO and VAO
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Model::update()
{
	// Spin the cube by 1 degree
	//spin(0.1f);
}

void Model::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::normalize()
{
	glm::vec3 points_max = vertices[0];
	glm::vec3 points_min = vertices[0];
	for (int i = 1; i < vertices.size(); i++) {
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
	for (int i = 0; i < vertices.size(); i++)
	{
		GLfloat distance = glm::length(vertices[i] - centroid);
		if (distance > distance_max)
			distance_max = distance;
	}
	GLfloat scale_factor = 10.0f / distance_max;
	
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// take the identity matrix and apply translation to it
	model = glm::translate(model, -centroid);
	model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
}

void Model::resize(double offset)
{
	model = glm::scale(model, glm::vec3(1.0f + offset * 0.02f));
}

void Model::rotate(glm::vec3 start, glm::vec3 end)
{
	glm::mat4 R = glm::rotate(0.02f, glm::cross(start, end));
	model = R * model;
}