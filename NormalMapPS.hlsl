#include "LightStructs.hlsli"
#include "GenStructs.hlsli"
#include "HelperFunctions.hlsli"

Texture2D normalMap : register(t0);
Texture2D diffuseTexture : register(t1);
SamplerState samplerOptions : register(s0);

cbuffer ExternalData : register(b0) {
	PointLight light1;
	DirectionalLight light2;
	DirectionalLight light3;
	float specular;
	float3 position;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel_NormalMap input) : SV_TARGET
{
	float3 unpackedNormal = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	float3x3 TBN = CalculateTBN(input.normal, input.tangent);

	input.normal = mul(unpackedNormal, TBN);

	float3 final = CalculateLight(input.normal, input.color, position, input.worldPos, specular, light1, input.uv, diffuseTexture, samplerOptions);
	final += CalculateLight(input.normal, input.color, position, input.worldPos, specular, light2, input.uv, diffuseTexture, samplerOptions);
	final += CalculateLight(input.normal, input.color, position, input.worldPos, specular, light3, input.uv, diffuseTexture, samplerOptions);
	return float4(final, 1);
}