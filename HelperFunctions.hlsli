#ifndef _GGP_HELPER_INCLUDES_
#define _GGP_HELPER_INCLUDES_


float3 Phong(float3 cameraPos, float3 worldPos, float3 lightDir, float3 normal, float exp) {
	float3 V = normalize(cameraPos - worldPos);
	float3 R = reflect(lightDir, normal);
	return pow(saturate(dot(R, V)), exp);
}

float3 CalculateLight(float3 normal, float4 color, float3 cameraPos, float3 worldPos, float exp, DirectionalLight light, float2 uv, Texture2D diffuseTexture, SamplerState samplerOptions) {
	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, uv).rgb;
	float3 toLight = normalize(light.Direction);
	float3 amount = saturate(dot(normal, toLight));
	float3 final = color * (amount * light.DiffuseColor + light.AmbientColor);
	if (exp > 0)
		final = surfaceColor * color * (amount * light.DiffuseColor + light.AmbientColor) + Phong(cameraPos, worldPos, light.Direction, normal, exp);
	else
		final = surfaceColor * color * (amount * light.DiffuseColor + light.AmbientColor);
	return final;
}

float3 CalculateLight(float3 normal, float4 color, float3 cameraPos, float3 worldPos, float exp, PointLight light, float2 uv, Texture2D diffuseTexture, SamplerState samplerOptions) {
	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, uv).rgb;
	float3 lightDir = normalize(worldPos - light.Position);
	float3 amount = saturate(dot(normal, lightDir));
	float3 final = color * (amount * light.DiffuseColor + light.AmbientColor);
	if (exp > 0)
		final = surfaceColor * color * (amount * light.DiffuseColor + light.AmbientColor) + Phong(cameraPos, worldPos, lightDir, normal, exp);
	else
		final = surfaceColor * color * (amount * light.DiffuseColor + light.AmbientColor);
	return final;
}

float3x3 CalculateTBN(float3 normal, float3 tangent) {
	float3 T = normalize(tangent - normal * dot(tangent, normal));
	float3 B = cross(tangent, normal);
	return float3x3(T, B, normal);
}

#endif