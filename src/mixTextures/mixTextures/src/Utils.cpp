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
	//fstream ���ж��Ƿ�ɹ����ļ�
	//https://blog.csdn.net/fruitz/article/details/41624921
	std::ifstream myfile(strSource);
	//��ʧ��
	if (myfile.fail()) {
		std::cout << "Can not open it " << std::endl;
		throw new runtime_error("IO stream corrupted");
	}
	//ת�����ַ���
	std::string str((std::istreambuf_iterator<char>(myfile)),
		std::istreambuf_iterator<char>());
	//�ر��ļ�������
	myfile.close();
	//str���鳤��һ��Ҫ +1,
	/*ԭ��: https://blog.csdn.net/ShiQW5696/article/details/80676290 */
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