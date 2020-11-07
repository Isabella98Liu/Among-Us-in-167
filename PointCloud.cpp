#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */

	nextObj = NULL;
	morph_tag = MORPHING_RATE;

	if (objFilename != "") {
		std::ifstream objFile(objFilename); // The obj file we are reading. 
		// Check whether the file can be opened.
		if (objFile.is_open())
		{
			std::string line; // A line in the file.

			// Read lines from the file.
			while (std::getline(objFile, line))
			{
				// Turn the line into a string stream for processing.
				std::stringstream ss;
				ss << line;

				// Read the first word of the line.
				std::string label;
				ss >> label;

				// If the line is about vertex (starting with a "v").
				if (label == "v")
				{
					// Read the later three float numbers and use them as the 
					// coordinates.
					glm::vec3 point;
					ss >> point.x >> point.y >> point.z;

					// Process the point. For example, you can save it to a.
					points.push_back(point);
				}
			}
		}
		else
			std::cerr << "Can't open the file " << objFilename << std::endl;
		objFile.close();
	}
	else
	{
		for (int i = 0; i < CUBE_SIZE; i++) {
			glm::vec3 point = glm::vec3(getRandomFloat() * CUBE_SCALE, getRandomFloat() * CUBE_SCALE, getRandomFloat() * CUBE_SCALE);
			points.push_back(point);
		}

	}


	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */

	// First calculate the centroid of the model
	glm::vec3 points_max = points[0];
	glm::vec3 points_min = points[0];
	for (int i = 1; i < points.size(); i++) {
		if (points[i].x < points_min.x)
			points_min.x = points[i].x;
		else if (points[i].x > points_max.x)
			points_max.x = points[i].x;

		if (points[i].y < points_min.y)
			points_min.y = points[i].y;
		else if (points[i].y > points_max.y)
			points_max.y = points[i].y;

		if (points[i].z < points_min.z)
			points_min.z = points[i].z;
		else if (points[i].z > points_max.z)
			points_max.z = points[i].z;
	}
	glm::vec3 centroid = glm::vec3((points_min.x + points_max.x) / 2.0f, (points_min.y + points_max.y) / 2.0f, (points_min.z + points_max.z) / 2.0f);
	// Get the max distance of any point to the centroid
	GLfloat distance_max = 0.0f;
	for (int i = 0; i < points.size(); i++)
	{
		GLfloat distance = glm::length(points[i] - centroid);
		if (distance > distance_max)
			distance_max = distance;
	}
	GLfloat scale_factor = 10.0f / distance_max;
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// take the identity matrix and apply translation to it
	//model = glm::translate(model, -centroid);
	//model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));

	for (int i = 0; i < points.size(); i++)
	{
		points[i] -= centroid;
		points[i] *= scale_factor;
	}

	// Set the color. 
	color = glm::vec3(0, 255, 222);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
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

	// Set point size
	glPointSize(pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, points.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree
	spin(0.1f);
	
	// if there is a last obj, do morphing transform
	if (nextObj)
		morphing();
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	if ((pointSize+size) > 0.0f)
		pointSize += size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::morphing()
{
	// Update the liner interpolation of points transformation
	std::vector<glm::vec3> next_points = nextObj->getPoints();

	int size_old = points.size();
	int size_new = next_points.size();

	if (size_new < size_old)
		for (int i = 0; i < size_old - size_new; i++)
			points.pop_back();
	else
		for (int i = 0; i < size_new - size_old; i++)
			points.push_back(glm::vec3(getRandomFloat()* CUBE_SCALE, getRandomFloat()* CUBE_SCALE, getRandomFloat()* CUBE_SCALE));

	for (int i = 0; i < points.size(); i++) {
		points[i] += (next_points[i] - points[i]) / (GLfloat)morph_tag;
	}
	morph_tag--;

	// update the buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// if the morphing has been finished, reset the last object to NULL
	if (morph_tag == 0)
	{
		morph_tag = MORPHING_RATE;
		nextObj = NULL;
	}

}
