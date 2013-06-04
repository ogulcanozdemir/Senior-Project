/*
 * ARGraphics.cpp
 *
 *  Created on: May 7, 2013
 *      Author: oulcan
 */

#include "ARLocal.h"

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define DEBUG 0
#define LOG_TAG "ARGraphics"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

GLuint m_backgroundTextureId;
GLuint defaultFrameBuffer, colorRenderBuffer, depthRenderBuffer;

GLint width = 1280;
GLint height = 720;

GLfloat w = (GLfloat) width;
GLfloat h = (GLfloat) height;

Matrix44 projectionMatrix;

void drawBackground()
{
	const GLfloat squareVertices[] =
	{
			0, 0,
			w, 0,
			0, h,
			w, h
	};

	static const GLfloat textureVertices[] =
	{
			1, 0,
			1, 1,
			0, 0,
			0, 1
	};

	static const GLfloat proj[] =
	{
			0, -2.f/w, 0, 0,
			-2.f/h, 0, 0, 0,
			0, 0, 1, 0,
			1, 1, 0, 1
	};

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthMask(FALSE);
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);

	glVertexPointer(2, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textureVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor4f(1, 1, 1, 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0 , 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void buildProjectionMatrix(ARCameraCalibration calibration, int width, int height, Matrix44& projectionMatrix)
{
	float near = 0.01;
	float far = 100;
/*
	float f_x = calibration.m_intrinsic.matrix[0][0];
	float f_y = calibration.m_intrinsic.matrix[1][1];
	float c_x = calibration.m_intrinsic.matrix[0][2];
	float c_y = calibration.m_intrinsic.matrix[1][2];
*/
	float f_x = calibration.getIntrinsic().data[0];
	float f_y = calibration.getIntrinsic().data[4];
	float c_x = calibration.getIntrinsic().data[2];
	float c_y = calibration.getIntrinsic().data[5];

	//LOGD("f_x => %f\nf_y => %f\nc_x => %f\nc_y =>%f", f_x, f_y, c_x, c_y);
/*
	projectionMatrix.data[0] = -2.0 * f_x / width;
	projectionMatrix.data[1] = 0.0;
	projectionMatrix.data[2] = 0.0;
	projectionMatrix.data[3] = 0.0;

	projectionMatrix.data[4] = 0.0;
	projectionMatrix.data[5] = 2.0 * f_y / height;
	projectionMatrix.data[6] = 0.0;
	projectionMatrix.data[7] = 0.0;

	projectionMatrix.data[8] = 2.0 * c_x / width - 1.0;
	projectionMatrix.data[9] = 2.0 * c_y / height - 1.0;
	projectionMatrix.data[10] = -(far + near) / (far - near);
	projectionMatrix.data[11] = -1.0;

	projectionMatrix.data[12] = 0.0;
	projectionMatrix.data[13] = 0.0;
	projectionMatrix.data[14] = -2.0 * far * near / (far - near);
	projectionMatrix.data[15] = 0.0;
	*/
	projectionMatrix.data[0] = 2.0 * f_x / width;
	projectionMatrix.data[1] = 0.0;
	projectionMatrix.data[2] = 0.0;
	projectionMatrix.data[3] = 0.0;

	projectionMatrix.data[4] = 0.0;
	projectionMatrix.data[5] = 2.0 * f_y / height;
	projectionMatrix.data[6] = 0.0;
	projectionMatrix.data[7] = 0.0;

	projectionMatrix.data[8] = 2.0 * (c_x / width) - 1.0;
	projectionMatrix.data[9] = 2.0 * (c_y / height) - 1.0;
	projectionMatrix.data[10] = -(far + near) / (far - near);
	projectionMatrix.data[11] = -1.0;

	projectionMatrix.data[12] = 0.0;
	projectionMatrix.data[13] = 0.0;
	projectionMatrix.data[14] = -2.0 * far * near / (far - near);
	projectionMatrix.data[15] = 0.0;
}

void updateBackground(cv::Mat& input)
{
	glViewport(0, 0, width, height);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, input.cols, input.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, input.data);

}
