#pragma once
#include "DXCore.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <fstream>
#include <vector>
class Mesh
{
public:
	Mesh(
		Vertex* vertices, 
		int vertNum, 
		UINT* indices,
		int indNum, 
		Microsoft::WRL::ComPtr<ID3D11Device> device
	);
	Mesh(const char* file, Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh();
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
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	int indiceCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};