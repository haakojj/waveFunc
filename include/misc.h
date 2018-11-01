#pragma once

GLFWwindow* initWindow(const int width, const int height, const char* name);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
unsigned int roundNextPow2(unsigned int x);
unsigned char clamp(float x);