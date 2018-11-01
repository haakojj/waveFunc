#include "libs.h"
#include "myWindow.h"

MyWindow::MyWindow(char* windowName, int width, int height)
{
	this->windowName = windowName;
	this->windowWidth = width;
	this->windowHeight = height;
	this->textureWidth = roundNextPow2(width);
	this->textureHeight = roundNextPow2(height);
	this->window = initWindow(this->windowWidth, this->windowHeight, this->windowName);
	// if (window == nullptr) return -1;

	GLfloat g_vertex_buffer_data[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

	float rationW = (float)this->windowWidth / (float)this->textureWidth;
	float rationH = (float)this->windowHeight / (float)this->textureHeight;
	GLfloat g_uv_buffer_data[] = {
		rationW ,0.0f,
		0.0f,0.0f,
		rationW ,rationH,
		0.0f,rationH,
		rationW ,rationH,
		0.0f,0.0f

	};

	glm::mat4 Projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 5.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::mat4(1.0f);
	this->mvp = Projection * View * Model;

	glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	this->outputProgramID = LoadShaders("passthrough.vertexshader", "output.fragmentshader");
	this->output_mvpID = glGetUniformLocation(this->outputProgramID, "MVP");
	this->output_CurrentStateTextureID = glGetUniformLocation(this->outputProgramID, "myTextureSampler");

	this->wavefuncProgramID = LoadShaders("passthrough.vertexshader", "wavefunc.fragmentshader");
	this->wavefunc_mvpID = glGetUniformLocation(this->wavefuncProgramID, "MVP");
	this->wavefunc_dtID = glGetUniformLocation(this->wavefuncProgramID, "dt");
	this->wavefunc_CurrentStateTextureID = glGetUniformLocation(this->wavefuncProgramID, "currentStateSampler");
	this->wavefunc_OldStateTextureID = glGetUniformLocation(this->wavefuncProgramID, "oldStateSampler");

	this->heatfuncProgramID = LoadShaders("passthrough.vertexshader", "heatfunc.fragmentshader");
	this->heatfunc_mvpID = glGetUniformLocation(this->heatfuncProgramID, "MVP");
	this->heatfunc_dtID = glGetUniformLocation(this->heatfuncProgramID, "dt");
	this->heatfunc_CurrentStateTextureID = glGetUniformLocation(this->heatfuncProgramID, "currentStateSampler");

	glGenVertexArrays(1, &this->VertexArrayID);
	glBindVertexArray(this->VertexArrayID);

	glGenBuffers(1, &this->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &this->uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	this->framebuffer = 0;
	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

	this->glCurrentStateTexture = 0;
	this->texture = new float[this->windowWidth * this->windowHeight];
	glGenTextures(1, &this->glCurrentStateTexture);
	glBindTexture(GL_TEXTURE_2D, this->glCurrentStateTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, this->textureWidth, this->textureHeight, 0, GL_RED, GL_FLOAT, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	this->glOldStateTexture = 1;
	glGenTextures(1, &this->glOldStateTexture);
	glBindTexture(GL_TEXTURE_2D, this->glOldStateTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, this->textureWidth, this->textureHeight, 0, GL_RED, GL_FLOAT, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	this->glRenderedTexture = 2;
	glGenTextures(1, &this->glRenderedTexture);
	glBindTexture(GL_TEXTURE_2D, this->glRenderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, this->textureWidth, this->textureHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->glRenderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
}

MyWindow::~MyWindow()
{
	glfwTerminate();
	delete[] this->texture;
}

void MyWindow::update() {
	glBindTexture(GL_TEXTURE_2D, this->glCurrentStateTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->windowWidth, this->windowHeight, GL_RED, GL_FLOAT, this->texture);
	glBindTexture(GL_TEXTURE_2D, this->glOldStateTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->windowWidth, this->windowHeight, GL_RED, GL_FLOAT, this->texture);
}

void MyWindow::draw(float dt) {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->glCurrentStateTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->glOldStateTexture);
	


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(this->outputProgramID);
	glUniformMatrix4fv(output_mvpID, 1, GL_FALSE, &this->mvp[0][0]);
	glUniform1i(this->output_CurrentStateTextureID, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glfwSwapBuffers(this->window);



	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(this->wavefuncProgramID);
	glUniformMatrix4fv(wavefunc_mvpID, 1, GL_FALSE, &this->mvp[0][0]);
	glUniform1i(this->wavefunc_CurrentStateTextureID, 0);
	glUniform1i(this->wavefunc_OldStateTextureID, 1);
	glUniform1f(wavefunc_dtID, dt);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	GLuint tmp = this->glOldStateTexture;
	this->glOldStateTexture = this->glCurrentStateTexture;
	this->glCurrentStateTexture = glRenderedTexture;
	this->glRenderedTexture = tmp;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->glCurrentStateTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->glOldStateTexture);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->glRenderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);







	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(this->heatfuncProgramID);
	glUniformMatrix4fv(heatfunc_mvpID, 1, GL_FALSE, &this->mvp[0][0]);
	glUniform1i(this->heatfunc_CurrentStateTextureID, 0);
	glUniform1f(heatfunc_dtID, dt);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	tmp = this->glCurrentStateTexture;
	this->glCurrentStateTexture = glRenderedTexture;
	this->glRenderedTexture = tmp;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->glCurrentStateTexture);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->glRenderedTexture, 0);
	DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);



	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void MyWindow::poolEvents() {
	glfwPollEvents();
}

float* MyWindow::getTexture() {
	return this->texture;
}

int MyWindow::getWidth() {
	return this->windowWidth;
}

int MyWindow::getHeight() {
	return this->windowHeight;
}

GLFWwindow* MyWindow::getWindow() {
	return this->window;
}

void MyWindow::setPixel(unsigned int x, unsigned int y, float v) {
	this->texture[y*this->windowWidth + x] = v;
}