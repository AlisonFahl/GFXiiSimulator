#version 430

layout (location = 0) uniform mat4 model;	//model matrix
layout (location = 1) uniform mat4 view;	//view matrix
layout (location = 2) uniform mat4 projection;	//projection matrix
layout (location = 3) uniform float texDisp;

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec2 texCoord;

out vec2 outUV;

void main()
{	
	gl_Position = projection * view * model * position;

	outUV = texCoord + vec2(-1, 0) * texDisp;
}
