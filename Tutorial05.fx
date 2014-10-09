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
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
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
    float rad = sqrt(x * x + y * y + z * z);
    output.Pos.x = (input.Pos.x / rad) * (1 - time) + (input.Pos.x) * time;
    output.Pos.y = (input.Pos.y / rad) * (1 - time) + (input.Pos.y) * time;
    output.Pos.z = (input.Pos.z / rad) * (1 - time) + (input.Pos.z) * time;
    output.Pos.w = 1;
    output.Pos = mul( output.Pos, Matrix );

    //output.Pos = mul( output.Pos, View );
    //output.Pos = mul( output.Pos, Projection );

    output.Color = input.Color;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    return input.Color;
}
