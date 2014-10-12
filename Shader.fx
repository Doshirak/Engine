//--------------------------------------------------------------------------------------
// File: Tutorial05.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    matrix Matrix;
    float time;
	int flag;
	float PHI;
	float4 ambientColor;
	float4 vLightDir[2];
	float4 vLightColor[2];
	float4 vOutputColor;
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float4 Color : COLOR0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    float x = input.Pos.x;
    float y = input.Pos.y;
    float z = input.Pos.z;
	
	float angle1 = atan2(x, z);
	float sin1 = sin(angle1 + time);
	float sign1 = sign(sin1);

	float angle2 = atan2(z, x);
	float sin2 = sin(angle2);
	float sign2 = sign(sin2);

	float angle3 = atan2(x, y);
	float sin3 = sin(angle3);
	float sign3 = sign(sin3);

	float agnle4 = PHI / 4;

    float rad = sqrt(x * x + y * y + z * z);

	if (flag == 1) {
		output.Pos.x = (input.Pos.x / rad) * (1 - time) + (input.Pos.x) * time;
		output.Pos.y = (input.Pos.y / rad) * (1 - time) + (input.Pos.y) * time;
		output.Pos.z = (input.Pos.z / rad) * (1 - time) + (input.Pos.z) * time;
	}
	else {
		float x2 = (abs(angle1) > agnle4) ? rad * sin(agnle4) * sign(x) : x;
		float z2 = (abs(angle2) > agnle4) ? rad * sin(agnle4) * sign(z) : z;
		float y2 = (abs(angle3) > agnle4) ? rad * sin(agnle4) * sign(y) : y;
		output.Pos.x = x2 * (1 - time) + x * time;
		output.Pos.z = z2 * (1 - time) + z * time;
		output.Pos.y = y2 * (1 - time) + y * time;
	}
    output.Pos.w = 1;
    output.Pos = mul( output.Pos, Matrix );
	output.Norm = mul(input.Norm, World);
	output.Color = input.Color;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float x = input.Pos.x;
	float y = input.Pos.y;
	float Shininess = 1;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float SpecularIntensity = 1;
	float3 ViewVector = float3(1, 0, 0);

	float3 light = normalize(vLightDir[0]);
	float3 normal = normalize(input.Norm);
	float3 r = normalize(2 * dot(light, normal) * normal - light);
	float3 v = normalize(mul(normalize(ViewVector), World));
	
	float dotProduct = dot(r, v);
	float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * length(input.Color);

	float4 finalColor = input.Color;

	finalColor += ambientColor;

	//do NdotL lighting for 2 lights
	for (int i = 0; i<2; i++)
	{
		//difuse
		finalColor += saturate(dot((float3)vLightDir[i], input.Norm) * vLightColor[i]);
	}

	float4 diffuse = dot((float3)vLightDir[0], input.Norm) * vLightColor[0];

	finalColor = saturate(input.Color + specular + ambientColor + diffuse);
	finalColor.a = 1.0f;

	return finalColor;

	//return input.Color;
}

