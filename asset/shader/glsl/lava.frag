#version 430

layout (binding = 0) uniform sampler2D texColour;

layout (location = 0) out vec4 outFrag;

in vec2 outUV;

void main()
{
	outFrag = texture(texColour, outUV);
}
