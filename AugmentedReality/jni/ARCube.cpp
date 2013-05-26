/*
 * ARCube.cpp
 *
 *  Created on: May 26, 2013
 *      Author: oulcan
 */

#include "ARCube.hpp"
#include <string.h>

ARCube::ARCube(GLfloat pVertices[], GLbyte pIndices[]) {

	memcpy(vertices, pVertices, 108 * sizeof(GLfloat));
	memcpy(indices, pIndices, 6 * sizeof(GLbyte));
}

void ARCube::draw(GLuint pvPositionHandle) {
	glVertexAttribPointer(pvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(pvPositionHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

ARCube::~ARCube() {

}


