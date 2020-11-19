#include "Sphere.h"

// Reference: http://www.songho.ca/opengl/gl_sphere.html#:~:text=In%20order%20to%20draw%20the,triangle%20strip%20cannot%20be%20used.

/*
Sphere::Sphere(unsigned int stackCount, unsigned int sectorCount, unsigned int scale)
{
	unsigned int indicator = 0;

	// Generate Sphere
	for (unsigned int i = 0; i < stackCount; i++)
	{
		double bottom_lat = glm::pi<double>() * (-0.5 + (double)i / stackCount);
		double up_lat = glm::pi<double>() * (-0.5 + (double)(i + 1) / stackCount);

		double bottom_r = cos(bottom_lat);
		double bottom_z = sin(bottom_lat);

		double up_r = cos(up_lat);
		double up_z = sin(up_lat);


		for (unsigned int j = 0; j < sectorCount; j++)
		{
			double left_lng = 2 * glm::pi<double>() * (double)(j / sectorCount);
			double right_lng = 2 * glm::pi<double>() * (double)((j + 1) / sectorCount);

			glm::vec3 A;
			A.x = up_r * cos(left_lng);
			A.y = up_r * sin(left_lng);
			A.z = up_z;
			A = A * (GLfloat)scale;
			vertices.push_back(A);

			glm::vec3 B;
			B.x = bottom_r * cos(left_lng);
			B.y = bottom_r * sin(left_lng);
			B.x = bottom_z;
			B = B * (GLfloat)scale;
			vertices.push_back(B);

			glm::vec3 C;
			C.x = bottom_r * cos(right_lng);
			C.y = bottom_r * sin(right_lng);
			C.x = bottom_z;
			C = C * (GLfloat)scale;
			vertices.push_back(C);

			glm::vec3 D;
			D.x = up_r * cos(right_lng);
			D.y = up_r * sin(right_lng);
			D.z = up_z;
			D = D * (GLfloat)scale;
			vertices.push_back(D);

			glm::vec3 normal = glm::normalize((A + B + C + D) * 0.25f);
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);

			glm::ivec3 indice = glm::ivec3(indicator, indicator + 1, indicator + 2);
			indices.push_back(indice);
			indice = glm::ivec3(indicator + 2, indicator + 3, indicator);
			indices.push_back(indice);
		}
		indicator += 4;
	}

	model = glm::mat4(1);

	for (int i = 0; i < vertices.size(); i++)
	{
		printf(" vertex %d: [ %f, %f, %f ] \n", i, vertices[i].x, vertices[i].y, vertices[i].z);
	}

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

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
*/

Sphere::Sphere(unsigned int stackCount, unsigned int sectorCount, unsigned int scale) {
	int i, j;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> normals;
	int indicator = 0;
	for (i = 0; i <= stackCount; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / stackCount);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / stackCount);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		for (j = 0; j <= sectorCount; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / sectorCount;
			double lng1 = 2 * glm::pi<double>() * (double)(j) / sectorCount;

			double x1 = cos(lng1);
			double y1 = sin(lng1);
			double x = cos(lng);
			double y = sin(lng);

			vertices.push_back(x * zr1 * scale);
			vertices.push_back(y * zr1 * scale);
			vertices.push_back(z1 * scale);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr0 * scale);
			vertices.push_back(y * zr0 * scale);
			vertices.push_back(z0 * scale);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x1 * zr0 * scale);
			vertices.push_back(y1 * zr0 * scale);
			vertices.push_back(z0 * scale);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x1 * zr1 * scale);
			vertices.push_back(y1 * zr1 * scale);
			vertices.push_back(z1 * scale);
			indices.push_back(indicator);
			indicator++;

			float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
			float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
			float z_avg = (2 * z0 + 2 * z1) / 4;

			glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

			normals.push_back(avg.x);
			normals.push_back(avg.y);
			normals.push_back(avg.z);

			normals.push_back(avg.x);
			normals.push_back(avg.y);
			normals.push_back(avg.z);

			normals.push_back(avg.x);
			normals.push_back(avg.y);
			normals.push_back(avg.z);

			normals.push_back(avg.x);
			normals.push_back(avg.y);
			normals.push_back(avg.z);
		}
		indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	numsToDraw = indices.size();
}

Sphere::~Sphere()
{
	// Delete the VBO/VEO and VAO
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader, GLuint texture, glm::vec3 eyePos)
{

	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(eyePos));

	// Bind the VAO
	glBindVertexArray(VAO);
	//glActiveTexture(texture);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_QUADS, numsToDraw, GL_UNSIGNED_INT, 0);

	//// Draw the points using triangles, indexed with the EBO
	//glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sphere::update()
{

}
