#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include <math.h>
#include "StrategyEngine.h"
class Mesh2D
{

public:
	Mesh2D(float width, float height, Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh2D();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
private:
	void CreateBuffers(
		Vertex* vertices,
		int vertNum,
		UINT* indices,
		int indNum,
		Microsoft::WRL::ComPtr<ID3D11Device> device
	);
	void CalculateTangents(
		Vertex* vertices,
		int vertNum,
		UINT* indices,
		int indNum
	);
	int indiceCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};
