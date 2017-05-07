#version 430

layout (location = 0) uniform mat4 model;	//model matrix
layout (location = 1) uniform mat4 view;	//view matrix
layout (location = 2) uniform mat4 projection;	//projection matrix
layout (location = 3) uniform vec4 lightpos;	//light position
layout (location = 4) uniform mat4 lightmatrix;	//light matrix
layout (location = 5) uniform mat4 lightproj;	//light projection matrix
layout (location = 6) uniform vec3 eyePosition;

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec4 inNormal;	//vertex attribute: normal
layout (location = 2) in vec2 inUV;		//vertex attribute: texcoords
layout (location = 3) in vec3 inTangent;

out vec3 outNormal;		//output: normal
out vec3 lightvec;		//output: light vector
out vec3 viewvec;		//output: view vector
out vec2 outUV;			//output: texcoords
out vec4 outPosInLight;	//output: vertex position in light space
out vec3 outTangent;

void main()
{	
	gl_Position = projection * view * model * position;
		
	outUV = inUV;
	outNormal = (model * vec4(inNormal.xyz, 0)).xyz;
	outTangent = (model * vec4(inTangent, 0)).xyz;
	viewvec = (model * vec4(position.xyz, 1)).xyz - eyePosition;
	lightvec = normalize(vec3(0, -3, -1));
}
