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

GLint framebufferWidth;
GLint framebufferHeight;

GLuint defaultFramebuffer;
GLuint colorRenderbuffer;
GLuint depthRenderbuffer;

GLuint backgroundTextureId;

void createFrameBuffer(cv::Mat& inputMat);
void deleteFrameBuffer();
void setFrameBuffer(cv::Mat& inputMat);
void drawFrame(cv::Mat& inputMat);
void drawBackground(cv::Mat& inputMat);
void buildProjectionMatrix(Matrix33& cameraMatrix, int height, int width, Matrix44& projectionMatrix);
void drawAR(cv::Mat& inputMat);
void updateBackground(cv::Mat& inputMat);


void createFrameBuffer(cv::Mat& inputMat)
{
	LOGD("width => %d, height => %d", inputMat.cols, inputMat.rows);

	glGenFramebuffers(1, &defaultFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

	glGenRenderbuffers(1, &colorRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &inputMat.cols);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &inputMat.rows);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, inputMat.cols, inputMat.rows);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOGD("failed to make complete frame buffer");

}

void deleteFrameBuffer()
{
	if (defaultFramebuffer) {
		glDeleteFramebuffers(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}

	if (colorRenderbuffer) {
		glDeleteRenderbuffers(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}

	if (depthRenderbuffer) {
		glDeleteRenderbuffers(1, &defaultFramebuffer);
		depthRenderbuffer = 0;
	}
}

void setFrameBuffer(cv::Mat& inputMat)
{
	if (!defaultFramebuffer)
		createFrameBuffer(inputMat);

	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
	glViewport(0, 0, inputMat.cols, inputMat.rows);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void initView()
{
	glGenTextures(1, &backgroundTextureId);
	glBindTexture(GL_TEXTURE_2D, backgroundTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_DEPTH_TEST);
}

void updateBackground(cv::Mat& inputMat)
{
	setFrameBuffer(inputMat);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, inputMat.cols, inputMat.rows, 0, GL_BGRA_IMG, GL_UNSIGNED_BYTE, inputMat.data);
}

void drawFrame(cv::Mat& inputMat)
{
	LOGD("setframebuffer()");
	setFrameBuffer(inputMat);
	LOGD("drawbackground()");
	drawBackground(inputMat);
	LOGD("drawAR()");
	drawAR(inputMat);

	int glErCode = glGetError();

	if (glErCode != GL_NO_ERROR)
		LOGD("GL error code = %d", glErCode);

}

void drawBackground(cv::Mat& inputMat)
{
	GLfloat w = (GLfloat)inputMat.cols;
	GLfloat h = (GLfloat)inputMat.rows;

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

	glDepthMask(GL_FALSE);
	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, backgroundTextureId);

	glVertexPointer(2, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textureVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor4f(1, 1, 1, 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void buildProjectionMatrix(Matrix33& cameraMatrix, int height, int width, Matrix44& projectionMatrix)
{
	float near = 0.01;
	float far = 100;

	float f_x = cameraMatrix.data[0];
	float f_y = cameraMatrix.data[4];

	float c_x = cameraMatrix.data[2];
	float c_y = cameraMatrix.data[5];

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
	projectionMatrix.data[14] = -2.0 * far * near / (far- near);
	projectionMatrix.data[15] = 0.0;
}

void drawAR(cv::Mat& inputMat)
{
	Matrix44 projectionMatrix;
	buildProjectionMatrix(calibration.m_intrinsic, inputMat.cols, inputMat.rows, projectionMatrix);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthMask(GL_TRUE);
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
		-0.5f, 0.5f,
		0.5f, 0.5f
	};

	const GLubyte squareColors[] = {
			255, 255, 0, 255,
			0, 255, 255, 255,
			0,   0,   0,   0,
			255, 0, 255, 255
	};

	for (size_t transIdx = 0; transIdx < m_transformation.size(); transIdx++) {
		const Transformation& trans = m_transformation[transIdx];

		Matrix44 glMat = trans.getInverted().getMat44();

		glLoadMatrixf(reinterpret_cast<const GLfloat*>(&glMat.data[0]));

		glVertexPointer(2, GL_FLOAT, 0, squareVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_COLOR_ARRAY);

		float scale = 0.5;
		glScalef(scale, scale, scale);

		glTranslatef(0, 0, 0.1f);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineX);
		glDrawArrays(GL_LINES, 0, 2);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineY);
		glDrawArrays(GL_LINES, 0, 2);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, lineZ);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}

