/*
 * ARCube.hpp
 *
 *  Created on: May 26, 2013
 *      Author: oulcan
 */

#ifndef ARCUBE_HPP_
#define ARCUBE_HPP_

#include <GLES2/gl2.h>
#include <GLES/gl.h>

class ARCube {

private:
	GLfloat vertices[24];
	GLfloat colors[32];
	GLbyte 	indices[36];

public:
	ARCube(GLfloat[], GLfloat[], GLbyte[]);
	void draw();
	void lightning();
	virtual ~ARCube();

};


#endif /* ARCUBE_HPP_ */
