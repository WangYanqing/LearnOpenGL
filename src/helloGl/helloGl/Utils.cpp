#include "Utils.h"


using namespace std;
Utils::Utils()
{
}


Utils::~Utils()
{
}


char* Utils::readShaderFile(char* strSource)
{
	//fstream 中判断是否成功打开文件
	//https://blog.csdn.net/fruitz/article/details/41624921
	std::ifstream myfile(strSource);
	//打开失败
	if (myfile.fail()) {
		std::cout << "Can not open it " << std::endl;
		throw new runtime_error("IO stream corrupted");
	}
	//转换成字符串
	std::string str((std::istreambuf_iterator<char>(myfile)),
		std::istreambuf_iterator<char>());
	//关闭文件处理器
	myfile.close();
	//str数组长度一定要 +1,
	/*原因: https://blog.csdn.net/ShiQW5696/article/details/80676290 */
	int len = str.length();
	char* result = new char[len];
	strcpy_s(result, len + 1, str.c_str());
	return result;
}


void Utils::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}