#version 330 core

//in vec4 vertexColor; //Color from vertex shader's output
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

//uniform vec4 ourColor; 
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //color = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	//color = vec4(vertexColor.r * 2, vertexColor.y + 1.0f, vertexColor.b, vertexColor.w);
	
	//color = vec4(ourColor, 1.0);
	//color = texture(ourTexture1, TexCoord); // * vec4(ourColor, 1.0);
	vec4 color1 = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.5);
	//if(color1.a <= 0)
	//	discard;
	
	color1 = texture(ourTexture1, TexCoord);
	vec4 clr2 = texture(ourTexture2, TexCoord);
	color1.a = clr2.r;
	color = color1;
}