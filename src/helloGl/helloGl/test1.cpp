

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "Utils.h"


using namespace std;


GLFWwindow* initWindow(int w, int h, char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//------------------------------------
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}


	//----Set view port
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	std::cout << "--Viewport: " << width << "x" << height << std::endl;

	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, Utils::key_callback); //Key events callback

	return window;
}


long createShader(char* pathShader, GLenum shaderType)
{
	GLuint vs = glCreateShader(shaderType);
	GLchar* vsSrc = Utils::readShaderFile(pathShader);
	glShaderSource(vs, 1, &vsSrc, NULL);
	glCompileShader(vs);

	GLint success;
	GLchar info[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vs, 512, NULL, info);
		cout << "Err: shader compile failed: \n";
		cout << "  type: " << shaderType << " path: " << pathShader << endl;
		cout << info << endl;
		return -1;
	}
	
	return vs;
}


long createProgram(char* pathVertexShader, char* pathFragmentShader)
{
	long vs = createShader(pathVertexShader, GL_VERTEX_SHADER);
	long fs = createShader(pathFragmentShader, GL_FRAGMENT_SHADER);

	if (vs < 0 || fs < 0){
		cout << "Fail creating shader program!\n";
		return -1;
	}

	long sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);

	GLint success;
	glGetProgramiv(sp, GL_LINK_STATUS, &success);
	if (!success){
		GLchar info[512];
		glGetProgramInfoLog(sp, 512, NULL, info);
		cout << "Err: creating shader program failed\n";
		cout << info;
		return -1;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return sp;
}


int main()
{
	//Init a window
	GLFWwindow* window = initWindow(800, 600, "Learning openGL");
	if (!window){
		cout << ">>Fail creating a window!\n";
		return -1;
	}

	long shaderProgram = createProgram("./vs.vert", "./fs.frag");
	if (shaderProgram < 0){
		cout << "Fail creating shader program!\n";
		return -1;
	}

	long shaderProgramYellow = createProgram("./vs_in_pos_clr.vert", "./fs1.frag");
	
	//----------------------------------------------------------
	GLfloat triangle1[] = {
		//----1st triangle
		//-1.0f, 0, 0,
		//0, 0, 0,
		//0, 1.0f, 0,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	GLfloat triangle2[] = {
		//----2nd triangle
		//0, -0.5, 0, 1.0f, 0.0f, 0.0f,
		//0.5, -0.5, 0, 0, 1.0f, 0,
		//0.5, 0, 0, 0, 0, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	//GLfloat triangle1[] = {
	//	-0.9f, -0.5f, 0.0f,  // Left 
	//	-0.0f, -0.5f, 0.0f,  // Right
	//	-0.45f, 0.5f, 0.0f,  // Top 
	//};
	//GLfloat triangle2[] = {
	//	0.0f, -0.5f, 0.0f,  // Left
	//	0.9f, -0.5f, 0.0f,  // Right
	//	0.45f, 0.5f, 0.0f   // Top 
	//};

	GLuint VBO[2], VAO[2];
	glGenBuffers(sizeof(VBO), VBO);
	glGenVertexArrays(sizeof(VAO), VAO);

	//---------------------
	// 1st triangle
	//---------------------
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	//---------------------
	// 2nd triangle
	//---------------------
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//GLfloat vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f,

	//	//----1st triangle
	//	-0.5f, 0, 0,
	//	0, 0, 0,
	//	0, 0.5, 0,
	//	//----2nd triangle
	//	-0, -0.5, 0,
	//	0.5, -0.5, 0,
	//	0.5, 0, 0,

	//	//----rectangle
	//	0.5f, 0.5f, 0.0f,   // 右上角
	//	0.5f, -0.5f, 0.0f,  // 右下角
	//	-0.5f, -0.5f, 0.0f, // 左下角
	//	-0.5f, 0.5f, 0.0f   // 左上角
	//};

	//GLuint indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};

	////索引对象
	//GLuint EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//----Game loop----
	while (!glfwWindowShouldClose(window)){
		//Events
		glfwPollEvents();

		//Rendering...
		glClearColor(0.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//--------------------------------------------------
		//Draw triangles
		GLint xoffset = glGetUniformLocation(shaderProgramYellow, "xoffset");
		glUniform2f(xoffset, 0.5f, 0);
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//----------------------------------------
		//Use uniform to change the value from shaders
		//----------------------------------------
		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
		//GLint colorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
		//glUseProgram(shaderProgramYellow);
		//glUniform4f(colorLocation, 0.0f, greenValue, 0.0f, 1.0f);


		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//Swap buffers
		glfwSwapBuffers(window);
	}

	//------------------------------------
	glDeleteVertexArrays(sizeof(VAO), VAO);
	glDeleteBuffers(sizeof(VBO), VBO);
	glfwTerminate();

	return 0;
}