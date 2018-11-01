#pragma once
#include "libs.h"
#include "misc.h"

class MyWindow
{
public:
	MyWindow(char* windowName, int width, int height);
	~MyWindow();

	void update();
	void draw(float dt);
	void poolEvents();

	void setPixel(unsigned int x, unsigned int y, float v);

	float* getTexture();
	int getWidth();
	int getHeight();
	GLFWwindow* getWindow();

private:
	int windowWidth;
	int windowHeight;

	int textureWidth;
	int textureHeight;

	char* windowName;

	float* texture;
	glm::mat4 mvp;

	GLFWwindow* window;
	GLuint VertexArrayID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint framebuffer;

	GLuint outputProgramID;
	GLuint wavefuncProgramID;
	GLuint heatfuncProgramID;

	GLuint glCurrentStateTexture;
	GLuint glOldStateTexture;
	GLuint glRenderedTexture;

	GLuint output_CurrentStateTextureID;
	GLuint wavefunc_CurrentStateTextureID;
	GLuint wavefunc_OldStateTextureID;
	GLuint wavefunc_RenderTextureID;
	GLuint heatfunc_CurrentStateTextureID;

	GLuint output_mvpID;
	GLuint wavefunc_mvpID;
	GLuint wavefunc_dtID;
	GLuint heatfunc_mvpID;
	GLuint heatfunc_dtID;
};