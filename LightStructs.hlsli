#ifndef _GGP_LIGHT_INCLUDES_
#define _GGP_LIGHT_INCLUDES_

struct DirectionalLight {
	float3 AmbientColor;
	//HLSL pads this automatically
	float3 DiffuseColor;
	//HLSL pads this automatically
	float3 Direction;
};
struct PointLight {
	float3 AmbientColor;
	//HLSL pads this automatically
	float3 DiffuseColor;
	//HLSL pads this automatically
	float3 Position;
	//HLSL pads this automatically
	float Range;
};

#endif