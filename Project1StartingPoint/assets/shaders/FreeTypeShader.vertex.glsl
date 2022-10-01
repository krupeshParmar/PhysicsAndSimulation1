#version 400

layout ( location = 0 ) in vec2 in_Vertex;
layout ( location = 1 ) in vec2 in_TexCoord;

out vec2 ex_TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	ex_TexCoord = in_TexCoord;
	
	vec4 pos = ModelMatrix * vec4(in_Vertex, 0.0f, 1.0f);
	gl_Position = ProjectionMatrix * pos;
	
	return;
}
