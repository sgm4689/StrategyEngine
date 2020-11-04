#include "Material.h"
Material::Material(XMFLOAT4 color, SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource,
	Microsoft::WRL::ComPtr<ID3D11SamplerState> state, float specular, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal) {
	this->pixelShader = pixelShader;
	this->vertexShader = vertexShader;
	V4_color = color;
	this->specular = specular;
	this->resource = resource;
	this->state = state;
	this->normal = normal;

	hasNormal = (normal != nullptr) ? hasNormal = true : hasNormal = false;
}

Material::~Material() {
	//Shaders are deleted in Game. because that's where they're created
}

XMFLOAT4 Material::GetColor() {
	return V4_color;
}

SimplePixelShader* Material::GetPixelShader() {
	return pixelShader;
}
SimpleVertexShader* Material::GetVertexShader() {
	return vertexShader;
}

float Material::GetSpecularExponent() {
	return specular;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetResource() {
	return resource;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetNormalMap() {
	return normal;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::GetState(){
	return state;
}

//Setters
void Material::SetColor(XMFLOAT4 color) {
	V4_color = color;
}

//Flags
bool Material::HasNormalMap() {
	return hasNormal;
}