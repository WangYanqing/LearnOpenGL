

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "Utils.h"


using namespace std;

int mainx()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//------------------------------------
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	//----Set view port
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	std::cout << "--Viewport: " << width << "x" << height << std::endl;

	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, Utils::key_callback); //Key events callback
	//----------------------------------------------------------

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,

		//----1st triangle
		-0.5f, 0, 0,
		0, 0, 0,
		0, 0.5, 0,
		//----2nd triangle
		-0, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0, 0,

		//----rectangle
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//索引对象
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//顶点数组对象
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//将顶点数据存入到一个缓冲对象里
	GLuint VBO;
	glGenBuffers(1, &VBO); //Generate a Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//------------------------------------------------------------------
	//Vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar* vertexShaderSource = Utils::readShaderFile("./vs.vert");
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Test success
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERR: vertex shader compile failed:\n" << infoLog << endl;
	}

	//Fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* fragmentShaderSource = Utils::readShaderFile("./fs.frag");
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Test success
	//GLint success;
	//GLchar infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERR: fragment shader compile failed:\n" << infoLog << endl;
	}

	//Shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Test success
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERR: shaderProgram compile failed:\n" << infoLog << endl;
	}

	//glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//------------------------------------------------------------------

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//解绑VAO
	glBindVertexArray(0);


	std::cout << "sizeof(vertices)=" << sizeof(vertices) << std::endl;
	//cout << readFile("./vs.vert") << endl;

	//----Game loop----
	while (!glfwWindowShouldClose(window)){
		//Events
		glfwPollEvents();

		//Rendering...
		glClearColor(0.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);

		//--------------------------------------------------
		//----Use index buffer obj draw rectangle
		/*glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //GL_FILL
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);*/


		//--------------------------------------------------
		//Draw triangles
		glBindVertexArray(VAO);
		//Draw
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //GL_FILL
		glDrawArrays(GL_TRIANGLES, 3, 6);
		glBindVertexArray(0);

		//Swap buffers
		glfwSwapBuffers(window);
	}

	//------------------------------------
	//int a = 0;
	//std::cin >> a;

	glfwTerminate();

	return 0;
}