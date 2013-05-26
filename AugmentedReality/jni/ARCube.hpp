/*
 * ARCube.hpp
 *
 *  Created on: May 26, 2013
 *      Author: oulcan
 */

#ifndef ARCUBE_HPP_
#define ARCUBE_HPP_

#include <GLES2/gl2.h>

class ARCube {

private:
	GLfloat vertices[108];
	GLbyte 	indices[6];

public:
	ARCube(GLfloat[], GLbyte[]);
	void draw(GLuint);
	virtual ~ARCube();

};


#endif /* ARCUBE_HPP_ */
