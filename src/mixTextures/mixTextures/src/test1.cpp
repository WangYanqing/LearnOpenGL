

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "Utils.h"
#include <SOIL.h>


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


void draw001_triangle(GLFWwindow* window, long shaderProgram){
	GLfloat vertices[] = {
		//-0.5f, -0.5f, 0.0f,
		//0.5f, -0.5f, 0.0f,
		//0.0f, 0.5f, 0.0f
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//1，绑定VAO
	glBindVertexArray(VAO);
	//2, 把顶点数据复制到缓冲中共gl使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//3, 设置顶点属性，gl如果解析数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//4，解绑VAO
	glBindVertexArray(0);


	//Game loop
	while (!glfwWindowShouldClose(window)){
		//Poll events
		glfwPollEvents();

		//Clear screen
		glClearColor(0.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLint xoffset = glGetUniformLocation(shaderProgram, "xoffset");
		glUniform2f(xoffset, 0.0f, 0);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void draw002_withShaders(GLFWwindow* window, long shaderProgramYellow){
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
		glUniform2f(xoffset, 0.0f, 0);
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		//glUseProgram(shaderProgramYellow);
		//glBindVertexArray(VAO[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//Swap buffers
		glfwSwapBuffers(window);
	}

	//------------------------------------
	glDeleteVertexArrays(sizeof(VAO), VAO);
	glDeleteBuffers(sizeof(VBO), VBO);
}


void draw003_texture(GLFWwindow* window, long shaderProgram){
	//Load img
	int imgw, imgh;
	unsigned char* image = SOIL_load_image("res/100_rgb.png", &imgw, &imgh, 0, SOIL_LOAD_RGBA);

	//Gen texture
	GLuint tex0, tex1;
	glGenTextures(1, &tex0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); //多级渐远纹理
	//图片已经绑定到纹理上去了，现在需要释放内存
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	image = NULL;
	image = SOIL_load_image("res/100_a.png", &imgw, &imgh, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLfloat vertices[] = {
		//-0.5f, -0.5f, 0.0f,
		//0.5f, -0.5f, 0.0f,
		//0.0f, 0.5f, 0.0f

		//(x, y, z), (r, g, b), (texx, texy)
		//0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // 右下
		//-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // 左下
		//0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f    // 顶部

		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	//1，绑定VAO
	glBindVertexArray(VAO);
	//2, 把顶点数据复制到缓冲中共gl使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//3, 设置顶点属性，gl如果解析数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);


	//4，解绑VAO
	glBindVertexArray(0);

	//混合函数
	glEnable(GL_BLEND); //开启混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Game loop
	while (!glfwWindowShouldClose(window)){
		//Poll events
		glfwPollEvents();

		//Clear screen
		glClearColor(0.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLint xoffset = glGetUniformLocation(shaderProgram, "xoffset");
		glUniform2f(xoffset, 0.0f, 0);

		//----bind texture----
		//glBindTexture(GL_TEXTURE_2D, tex0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);
		//--------

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //glDrawArrays(GL_TRIANGLES, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


int main()
{
	//Init a window
	GLFWwindow* window = initWindow(800, 600, "Learning openGL");
	if (!window){
		cout << ">>Fail creating a window!\n";
		return -1;
	}

	long shaderProgram = createProgram("res/vs.vert", "res/fs.frag");
	if (shaderProgram < 0){
		cout << "Fail creating shader program!\n";
		return -1;
	}

	long shaderProgramYellow = createProgram("res/vs_in_pos_clr.vert", "res/fs1.frag");
	
	draw003_texture(window, shaderProgramYellow);
	//draw001_triangle(window, shaderProgramYellow);
	//draw002_withShaders(window, shaderProgramYellow);
	
	glfwTerminate();

	return 0;
}