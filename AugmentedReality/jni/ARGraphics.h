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
#include <GLES2/gl2platform.h>
#include <GLES/glplatform.h>

#define DEBUG 0
#define LOG_TAG "ARGraphics"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

GLuint m_backgroundTextureId;
GLuint defaultFrameBuffer, colorRenderBuffer, depthRenderBuffer;

GLint width = 640;
GLint height = 480;

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

	float f_x = calibration.m_intrinsic.matrix[0][0];
	float f_y = calibration.m_intrinsic.matrix[1][1];
	float c_x = calibration.m_intrinsic.matrix[0][2];
	float c_y = calibration.m_intrinsic.matrix[1][2];

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
}

void drawAR()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthMask(TRUE);
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	glLineWidth(3.0f);

	float lineX[] = {0, 0, 0, 1, 0, 0};
	float lineY[] = {0, 0, 0, 0, 1, 0};
	float lineZ[] = {0, 0, 0, 0, 0, 1};

	const GLfloat squareVertices[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			-0.5f,  0.5f,
			 0.5f,  0.5f
	};

	const GLubyte squareColors[] = {
			255, 255,   0, 255,
			  0, 255, 255, 255,
			  0,   0,   0,   0,
			255,   0, 255, 255
	};

	for (size_t transformationIdx = 0; transformationIdx < m_transformation.size(); transformationIdx++) {
		const Transformation& trans = m_transformation[transformationIdx];

		Matrix44 glMatrix = trans.getMat44();

		glLoadMatrixf(reinterpret_cast<const GLfloat*>(&glMatrix.data[0]));
		/*
		LOGD("%f - %f - %f - %f", glMatrix.data[0], glMatrix.data[1], glMatrix.data[2], glMatrix.data[3]);
		LOGD("%f - %f - %f - %f", glMatrix.data[4], glMatrix.data[5], glMatrix.data[6], glMatrix.data[7]);
		LOGD("%f - %f - %f - %f", glMatrix.data[8], glMatrix.data[9], glMatrix.data[10], glMatrix.data[11]);
		LOGD("%f - %f - %f - %f", glMatrix.data[12], glMatrix.data[13], glMatrix.data[14], glMatrix.data[15]);
		 */
		glVertexPointer(2, GL_FLOAT, 0, squareVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_COLOR_ARRAY);

		float scale = 1.0;
		glScalef(scale, scale, scale);

		glTranslatef(0, 0, 0.1f);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineX);
		glDrawArrays(GL_LINES, 0, 2);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineY);
		glDrawArrays(GL_LINES, 0 , 2);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineZ);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}

void updateBackground(cv::Mat& input)
{
	glViewport(0, 0, width, height);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, input.cols, input.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, input.data);

}
