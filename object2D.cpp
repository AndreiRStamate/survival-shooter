#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;

	int vertexCount = 36;

	for (int i = 0; i < vertexCount; i++) {
		vertices.push_back(VertexFormat(corner + glm::vec3((double)length * .5l * cos(6.28318530717958 * i / vertexCount),
															(double) length * .5l * sin(6.28318530717958 * i / vertexCount),
															0), color));
	}

	Mesh* square = new Mesh(name);
	square->SetDrawMode(GL_TRIANGLE_FAN);

	std::vector<unsigned int> indices;
	for (int i = 1; i < vertexCount; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* object2D::CreatePlayer(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(length, 0, 0), color), //1
		VertexFormat(corner + glm::vec3(length, length, 0), color), //2
		VertexFormat(corner + glm::vec3(0, length, 0), color), //3

		VertexFormat(corner + glm::vec3(length/6, length/2+length/6, 0), glm::vec3(255/255, 85/255, 17/255)), //4
		VertexFormat(corner + glm::vec3(length/2-length/6, length / 2 + length / 6, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)), //5
		VertexFormat(corner + glm::vec3(length / 2 - length / 6, length/2 + length/3, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)), //6
		VertexFormat(corner + glm::vec3(length / 6, length / 2 + length / 3, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)),//7

		VertexFormat(corner + glm::vec3(length/2 + length / 6, length / 2 + length / 6, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)), //8
		VertexFormat(corner + glm::vec3(length - length / 6, length / 2 + length / 6, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)), //9
		VertexFormat(corner + glm::vec3(length - length / 6, length / 2 + length / 3, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)), //10
		VertexFormat(corner + glm::vec3(length/2 + length / 6, length / 2 + length / 3, 0), glm::vec3(255 / 255, 85 / 255, 17 / 255)),//11
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned int> indices
	{
		4, 5, 6,
		7, 4, 6,
		8, 9, 10,
		11, 8, 10,
		0, 1, 2,
		3, 0, 2
	};

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* object2D::CreateEnemy(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(length, 0, 0), color), //1
		VertexFormat(corner + glm::vec3(length, length, 0), color), //2
		VertexFormat(corner + glm::vec3(0, length, 0), color), //3

		VertexFormat(corner + glm::vec3(length / 6, length / 2 + length / 6, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //4
		VertexFormat(corner + glm::vec3(length / 2 - length / 6, length / 2 + length / 6, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //5
		VertexFormat(corner + glm::vec3(length / 2 - length / 6, length / 2 + length / 3, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //6
		VertexFormat(corner + glm::vec3(length / 6, length / 2 + length / 3, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)),//7

		VertexFormat(corner + glm::vec3(length / 2 + length / 6, length / 2 + length / 6, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //8
		VertexFormat(corner + glm::vec3(length - length / 6, length / 2 + length / 6, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //9
		VertexFormat(corner + glm::vec3(length - length / 6, length / 2 + length / 3, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)), //10
		VertexFormat(corner + glm::vec3(length / 2 + length / 6, length / 2 + length / 3, 0), glm::vec3(134 / 255, 25 / 255, 243 / 255)),//11
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned int> indices
	{
		4, 5, 6,
		7, 4, 6,
		8, 9, 10,
		11, 8, 10,
		0, 1, 2,
		3, 0, 2
	};

	square->InitFromData(vertices, indices);
	return square;
}
