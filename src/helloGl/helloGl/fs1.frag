#version 330 core

//in vec4 vertexColor; //Color from vertex shader's output
in vec3 ourColor;
out vec4 color;

//uniform vec4 ourColor; 

void main()
{
    //color = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	//color = vec4(vertexColor.r * 2, vertexColor.y + 1.0f, vertexColor.b, vertexColor.w);
	color = vec4(ourColor, 1.0);
}