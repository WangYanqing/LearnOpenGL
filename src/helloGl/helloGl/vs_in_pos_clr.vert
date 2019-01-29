#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform vec2 xoffset;

out vec3 ourColor; //Define a color output for fragment shader

void main()
{
	if(position.y > 0){	
		gl_Position = vec4(position.x, position.y - 1.0, position.z, 1.0);
	}else if(position.y < 0){
		gl_Position = vec4(position.x, position.y + 1.0, position.z, 1.0);
	}else{
		gl_Position = vec4(position, 1.0);
	}
	gl_Position = vec4(gl_Position.x + xoffset.x, gl_Position.y, gl_Position.z, 1.0);
}
	ourColor = color; //vec4(0.5f, 0.0f, 0.0f, 1.0f);