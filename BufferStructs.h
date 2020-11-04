#pragma once
#include <DirectXMath.h>

struct VSData {
	XMFLOAT4 colorTint;
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

};