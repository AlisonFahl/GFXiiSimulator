#version 430

layout (binding = 0) uniform sampler2D texColour;
layout (location = 4) uniform vec3 specularLightColor;

in vec2 outUV;
in vec4 outNormal;
in vec3 viewVec;
in vec3 lightVec;

layout (location = 0) out vec4 outFrag;

void main()
{
	vec3 normal = normalize(outNormal.xyz);

	vec3 eyeVec = normalize(viewVec);
	vec3 reflectionVec = reflect(lightVec, normal);

	vec4 difuseColor = texture(texColour, outUV);
	vec3 specularColor = vec3(0, 0, 0);

	float difuseValue = dot(normal, -lightVec);
	if(difuseValue > 0)
	{
		difuseColor = vec4(difuseValue * difuseColor.rgb, 1);

		float specValue = dot(eyeVec, -reflectionVec);;
		if(specValue > 0)
		{
			float specPower = 10;
			specValue = pow(specValue, specPower);

			specularColor = specularLightColor * specValue;
		}
	}
	else
	{
		difuseColor = vec4(difuseColor.rgb * 0.1, 1);
	}

	outFrag = clamp(difuseColor + vec4(specularColor, 0), 0, 1);
}
