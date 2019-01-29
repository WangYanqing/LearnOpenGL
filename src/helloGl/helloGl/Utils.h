
#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>

#pragma once
class Utils
{
public:
	Utils();
	~Utils();

	static char* readShaderFile(char*);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

