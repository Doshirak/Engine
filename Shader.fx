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
	float3 pointLightPos;
	float3 specularLightPos;
	int shininess;
	int attenuation;
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
	float4 origPos : TEXCOORD1;
	float4 Color : COLOR0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.origPos = input.Pos;
	output.Pos = mul(output.origPos, world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, projection);
	//output.Norm = mul(input.Norm, inversedWorld);
	output.Norm = input.Norm;
	output.Color = input.Color;
    return output;
}

float4 directional(PS_INPUT input)
{
	// SPECULAR
	float3 eyeNew = { 0.0f, 3.0f, -10.0f };
	float3 view = normalize(eye.xyz - mul(input.origPos, world));
		//float3 I = (lightDirection * -1);
		float3 h = normalize(view + lightDirection);
		//float r = I - 2 * dot(input.Norm, I) * input.Norm;

		//float dotProduct = dot(view, r);
		float dotProduct = dot(mul(input.Norm, inversedWorld), h);

	float4 specular;

	float dotProduct2 = dot(lightDirection, mul(input.Norm, inversedWorld));
	if (dotProduct2 > 0)
		//specular = max(pow(dotProduct, shininess), 0);
		specular = max(pow(dotProduct, 3), 0);
	//specular = dotProduct;
	else
		specular = 0;

	// DIFUSE 
	float4 diffuse = dot(normalize(lightDirection), mul(input.Norm, inversedWorld));
	float4 directional = saturate(saturate(input.Color * diffuseColor * diffuse) + saturate(input.Color * ambientColor) + saturate(input.Color * specularColor * specular));
	return directional;
}

float4 pointLight(PS_INPUT input)
{
	// SPECULAR
	float3 pLightPos = {0.0f, 0.0f, 0.0f};
	float3 newPos = mul(input.origPos, world);
	float distance = length(pLightPos - newPos);
	float3 pointLightDir = normalize(pLightPos - mul(input.origPos, world));
	float3 view = normalize(eye.xyz - mul(input.origPos, world));
		//float3 I = (lightDirection * -1);
	float3 h = normalize(view + pointLightDir);
		//float r = I - 2 * dot(input.Norm, I) * input.Norm;
		//float dotProduct = dot(view, r);
	float dotProduct = dot(mul(input.Norm, inversedWorld), h);

	float4 specular;
	float dotProduct2 = dot(pointLightDir, mul(input.Norm, inversedWorld));
	if (dotProduct2 > 0)
		//specular = max(pow(dotProduct, shininess), 0);
		specular = max(pow(dotProduct, 3), 0);
	//specular = dotProduct;
	else
		specular = 0;

	// DIFUSE 
	float4 diffuse = dot(normalize(pointLightDir), mul(input.Norm, inversedWorld));
	//diffuse = diffuse / (0.5 * distance);
	float4 pointLight = saturate(saturate(input.Color * diffuseColor * diffuse) + saturate(input.Color * ambientColor)/* + saturate(input.Color * specularColor * specular)*/);
	return pointLight;
}

float4 spotLight(PS_INPUT input)
{
	float3 spotDirection = { 3.0f, 0.0f, 0.0f };
	float3 spotPos = {-3.0f, 0.0f, 0.0f};

	float3 newPos = mul(input.origPos, world);
	float distance = length(spotPos - newPos);
	// SPECULAR
	float3 spotLightDir = normalize(spotPos - mul(input.origPos, world));
		float3 view = normalize(eye.xyz - mul(input.origPos, world));
		//float3 I = (lightDirection * -1);
		float3 h = normalize(view + spotLightDir);
		//float r = I - 2 * dot(input.Norm, I) * input.Norm;

		//float dotProduct = dot(view, r);
		float dotProduct = dot(mul(input.Norm, inversedWorld), h);

	float4 specular;
	float dotProduct2 = dot(spotLightDir, mul(input.Norm, inversedWorld));
	if (dotProduct2 > 0)
		//specular = max(pow(dotProduct, shininess), 0);
		specular = max(pow(dotProduct, 3), 0);
	//specular = dotProduct;
	else
		specular = 0;

	// DIFUSE 
	float dotProduct3 = max(dot(-1 * spotLightDir, spotDirection), 0);
	float4 diffuse = dot(spotLightDir, mul(input.Norm, inversedWorld)) * dotProduct3;
		float4 pointLight = saturate(saturate(input.Color * diffuseColor * diffuse) + saturate(input.Color * ambientColor)/* + saturate(input.Color * specularColor * specular)*/);
		return pointLight;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	return spotLight(input);
}

