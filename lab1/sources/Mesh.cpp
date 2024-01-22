#include "Mesh.h"
#include "Transform.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>           

// Standard Headers
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <glm/ext/matrix_transform.hpp>


Mesh::Mesh(aiMesh* aimesh) {
	for (int i = 0; i < aimesh->mNumVertices; i++) {
		vertices.push_back(glm::vec4(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z, 1));
	}
	for (int i = 0; i < aimesh->mNumFaces; i++) {
		if (aimesh->mFaces[i].mNumIndices != 3) {
			std::cout << "err: not a triangle" << std::endl;
			continue;
		}
		indices.insert(indices.end(), { aimesh->mFaces[i].mIndices[0],aimesh->mFaces[i].mIndices[1],aimesh->mFaces[i].mIndices[2] });
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	Mesh::normalize();
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::refreshBuffers() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertices.size(), (void*)(&vertices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), (void*)(&indices[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);

}

glm::mat3 Mesh::getBoundingBox() {
	float maxx, minx, maxy, miny, maxz, minz;
	maxx = minx = vertices[0].x; maxy = miny = vertices[0].y; maxz = minz = vertices[0].z;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x > maxx) maxx = vertices[i].x;
		if (vertices[i].x < minx) minx = vertices[i].x;
		if (vertices[i].y > maxy) maxy = vertices[i].y;
		if (vertices[i].y < miny) miny = vertices[i].y;
		if (vertices[i].z > maxz) maxz = vertices[i].z;
		if (vertices[i].z < minz) minz = vertices[i].z;
	}

	return glm::mat3(maxx, minx, (maxx + minx) / 2, maxy, miny, (maxy + miny) / 2, maxz, minz, (maxz + minz) / 2);
}

void Mesh::applyTransform(glm::mat4 transformation) {
	for (int i = 0; i < vertices.size(); i++) 
		vertices[i] = transformation * vertices[i];
}


void Mesh::normalize() {
	glm::mat3 maxminmid = Mesh::getBoundingBox();

	float maxrange = abs(maxminmid[0][0] - maxminmid[0][1]);
	if (maxrange < abs(maxminmid[1][0] - maxminmid[1][1]))maxrange = abs(maxminmid[1][0] - maxminmid[1][1]);
	if (maxrange < abs(maxminmid[2][0] - maxminmid[2][1]))maxrange = abs(maxminmid[2][0] - maxminmid[2][1]);

	maxrange = 2 / maxrange;
	applyTransform(glm::translate(glm::mat4(1), glm::vec3(-maxminmid[0][2], -maxminmid[1][2], -maxminmid[2][2])));
	applyTransform(glm::scale(glm::mat4(1), glm::vec3(maxrange, maxrange, maxrange)));
	refreshBuffers();
}
