/*
 * ARCube.cpp
 *
 *  Created on: May 26, 2013
 *      Author: oulcan
 */

#include "ARCube.hpp"
#include <string.h>

ARCube::ARCube(GLfloat pVertices[], GLfloat pColors[], GLbyte pIndices[]) {

	memcpy(vertices, pVertices, 24 * sizeof(GLfloat));
	memcpy(colors, pColors, 32 * sizeof(GLfloat));
	memcpy(indices, pIndices, 36 * sizeof(GLbyte));
}

void ARCube::draw() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ARCube::lightning() {
	GLfloat cubeOneAmbientFraction[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat cubeOneDiffuseFraction[4] = { 0.8, 0.0, 0.0, 1.0 };
	GLfloat cubeSpecularFraction[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat cubeEmissionFraction[4] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cubeOneAmbientFraction);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cubeOneDiffuseFraction);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cubeSpecularFraction);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cubeEmissionFraction);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);
}

ARCube::~ARCube() {

}

