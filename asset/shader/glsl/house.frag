#version 430

layout (binding = 0) uniform sampler2D texColour;
layout (binding = 1) uniform sampler2D texNormal;
layout (binding = 2) uniform sampler2D texSpecular;
layout (binding = 3) uniform sampler2D texEnv;
layout (binding = 4) uniform sampler2D shadowmap;

in vec3 outNormal;	//input: normal
in vec3 lightvec;	//input: light vector
in vec3 viewvec;	//input: view vector
in vec2 outUV;		//input: texcoords
in vec4 outPosInLight;	//input: position in light space
in vec3 outTangent;

layout (location = 0) out vec4 outFrag;

vec3 CalcFinalNormal()
{
    vec3 normal = normalize(outNormal);
    vec3 tangent = normalize(outTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 mapNormal = texture(texNormal, outUV).xyz;
    mapNormal = 2.0 * mapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    newNormal = TBN * mapNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}

void main()
{
	vec3 normal = CalcFinalNormal();

	vec3 eyeVec = normalize(viewvec);
	vec3 reflectionVec = reflect(lightvec, normal);

	vec4 difuseColor = texture(texColour, outUV);
	vec3 specularColor = vec3(0, 0, 0);

	float difuseValue = dot(normal, -lightvec);
	if(difuseValue > 0)
	{
		difuseColor = vec4(difuseValue * difuseColor.rgb, 1);

		float specValue = dot(eyeVec, -reflectionVec);;
		if(specValue > 0)
		{
			float specPower = (1 - texture(texSpecular, outUV).r) * 100;
			specValue = pow(specValue, specPower);

			specularColor = vec3(1, 1, 1) * specValue;
		}
	}
	else
	{
		difuseColor = vec4(difuseColor.rgb * 0.3, 1);
	}

	outFrag = clamp(difuseColor + vec4(specularColor, 0), 0, 1);
}
