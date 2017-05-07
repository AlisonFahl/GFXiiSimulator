#version 430

layout (location = 0) uniform mat4 model;	//model matrix
layout (location = 1) uniform mat4 view;	//view matrix
layout (location = 2) uniform mat4 projection;	//projection matrix
layout (location = 3) uniform vec3 viewVec;
layout (location = 4) uniform vec3 rootScale;

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 textureCoord;

out vec2 outUV;
out vec4 outNormal;

void main()
{	
	//Billboarding
	vec3 forwardVector = normalize(-viewVec);
	vec3 rightVector = normalize(cross(forwardVector, vec3(0, 1, 0)));
	vec3 upVector = cross(rightVector, forwardVector);

	mat4 modelTransform = mat4(vec4(rightVector * rootScale, 0), vec4(upVector * rootScale, 0), vec4(forwardVector * rootScale, 0), model[3]);

	gl_Position = projection * view * modelTransform * position;

	outNormal = normal;
	outUV = textureCoord;
}
