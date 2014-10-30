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
	matrix world;
	matrix view;
	matrix projection;
	matrix inversedWorld;
	float4 eye;
	float3 lightDirection;
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float shininess;
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
	output.Pos = input.Pos;
	output.Pos = mul(output.Pos, world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, projection);
	output.Norm = mul(input.Norm, world);
	output.Color = input.Color;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	matrix worldt = transpose(world);
	float3 view = normalize(eye - mul(input.Pos, world));
	float3 light = normalize(lightDirection);
	float3 normal = input.Norm;
	float3 h = normalize(view + light);
	float dotProduct = dot(normal, h);
	float4 specular;
	float dotProduct2 = dot(light, normal);
	if (dotProduct2 > 0)
		specular = max(pow(dotProduct, shininess), 0);
	else
		specular = 0;
	float4 diffuse = dot(normalize(lightDirection), input.Norm);
	return saturate(input.Color * (diffuse));
}

