#include "Mesh2D.h"

Mesh2D::Mesh2D(float width, float height, Microsoft::WRL::ComPtr<ID3D11Device> device) {
	Vertex vertices[] =
	{
		{ XMFLOAT3(+width / 2, +height / 2, +0.0f), XMFLOAT3(+width / 2, +height / 2, +0.0f), XMFLOAT3(0,0,0), XMFLOAT2(1,1)},
		{ XMFLOAT3(+width / 2, -height / 2, +0.0f), XMFLOAT3(+width / 2, -height / 2, +0.0f), XMFLOAT3(0,0,0), XMFLOAT2(1,1)},
		{ XMFLOAT3(-width / 2, -height / 2, +0.0f), XMFLOAT3(-width / 2, -height / 2, +0.0f), XMFLOAT3(0,0,0), XMFLOAT2(1,1)},
		{ XMFLOAT3(-width / 2, +height / 2, +0.0f), XMFLOAT3(-width / 2, +height / 2, +0.0f), XMFLOAT3(0,0,0), XMFLOAT2(1,1)},
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT indices[] = { 0, 1, 2 };

	CalculateTangents(vertices, 4, indices, 4);
	CreateBuffers(vertices, 4, indices, 4, device);
}

Mesh2D::~Mesh2D() {

}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh2D::GetIndexBuffer() {
	return indexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh2D::GetVertexBuffer() {
	return vertexBuffer;
}

int Mesh2D::GetIndexCount() {
	return indiceCount;
}

void Mesh2D::CreateBuffers(Vertex* vertices, int vertNum, UINT* indices, int indNum, Microsoft::WRL::ComPtr<ID3D11Device> device) {

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertNum;       // number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indNum;         // number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	indiceCount = indNum;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());
}

void Mesh2D::CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices)
{
	// Reset tangents
	for (int i = 0; i < numVerts; i++)
	{
		verts[i].Tangent = DirectX::XMFLOAT3(0, 0, 0);
	}

	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < numIndices;)
	{
		// Grab indices and vertices of first triangle
		unsigned int i1 = indices[i++];
		unsigned int i2 = indices[i++];
		unsigned int i3 = indices[i++];
		Vertex* v1 = &verts[i1];
		Vertex* v2 = &verts[i2];
		Vertex* v3 = &verts[i3];

		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;

		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;

		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;

		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;

		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);

		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;

		// Adjust tangents of each vert of the triangle
		v1->Tangent.x += tx;
		v1->Tangent.y += ty;
		v1->Tangent.z += tz;

		v2->Tangent.x += tx;
		v2->Tangent.y += ty;
		v2->Tangent.z += tz;

		v3->Tangent.x += tx;
		v3->Tangent.y += ty;
		v3->Tangent.z += tz;
	}

	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < numVerts; i++)
	{
		// Grab the two vectors
		DirectX::XMVECTOR normal = XMLoadFloat3(&verts[i].Normal);
		DirectX::XMVECTOR tangent = XMLoadFloat3(&verts[i].Tangent);

		// Use Gram-Schmidt orthogonalize
		tangent = DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(
				tangent, DirectX::XMVectorMultiply(
					normal, DirectX::XMVector3Dot(normal, tangent)
				)
			)
		);

		// Store the tangent
		XMStoreFloat3(&verts[i].Tangent, tangent);
	}
}