#version 430

layout (location = 0) uniform mat4 model;	//model matrix
layout (location = 1) uniform mat4 view;	//view matrix
layout (location = 2) uniform mat4 projection;	//projection matrix
layout (location = 3) uniform vec3 eyePosition;

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec4 normal;

out vec2 outUV;
out vec4 outNormal;
out vec3 viewVec;
out vec3 lightVec;

void main()
{	
	gl_Position = projection * view * model * position;

	outUV = textureCoord;
	outNormal = model * normal;
	viewVec = (model * vec4(position.xyz, 1)).xyz - eyePosition;
	lightVec = normalize(vec3(0, -3, -1));
}
