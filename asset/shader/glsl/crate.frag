#version 430

layout (binding = 0) uniform sampler2D texColour;

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
			float specPower = 600;
			specValue = pow(specValue, specPower);

			specularColor = vec3(1, 1, 1) * specValue;
		}
	}
	else
	{
		difuseColor = vec4(difuseColor.rgb * 0.1, 1);
	}

	outFrag = texture(texColour, outUV);//clamp(difuseColor + vec4(specularColor, 0), 0, 1);//vec4(normal, 1);
}
