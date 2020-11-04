#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
using namespace DirectX;
class Material
{
	public:
		Material(
			XMFLOAT4 color, 
			SimplePixelShader* pixelShader, 
			SimpleVertexShader* vertexShader,
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource,
			Microsoft::WRL::ComPtr<ID3D11SamplerState> state,
			float specular = 0,
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal = nullptr
		);
		~Material();

		//Getters
		XMFLOAT4 GetColor();
		SimplePixelShader* GetPixelShader();
		SimpleVertexShader* GetVertexShader();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResource();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNormalMap();
		Microsoft::WRL::ComPtr<ID3D11SamplerState> GetState();
		float GetSpecularExponent();

		//Setters
		void SetColor(XMFLOAT4 color);

		//Flags
		bool HasNormalMap();
	private:
		XMFLOAT4 V4_color;
		SimpleVertexShader* vertexShader;
		SimplePixelShader* pixelShader;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> state;
		
		bool hasNormal;

		float specular;

};

