#version 430

layout (binding = 0) uniform sampler2D texColour;

in vec4 outNormal;
in vec2 outUV;

layout (location = 0) out vec4 outFrag;

void main()
{
	outFrag = texture(texColour, outUV);
}
