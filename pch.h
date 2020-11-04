#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include <fstream>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>

typedef void (*func_p)(void)