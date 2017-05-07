#version 430

layout (location = 0) uniform mat4 model;	//model matrix
layout (location = 1) uniform mat4 projection;	//projection matrix

layout (location = 0) in vec3 position;	//vertex attribute: position
layout (location = 1) in vec2 inUV;		//vertex attribute: texcoords

out vec2 outUV;			//output: texcoords

void main()
{	
	gl_Position = projection * model * vec4(position, 1);
	outUV = inUV;
}
