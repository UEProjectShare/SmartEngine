#pragma once
#define CLOSE_DISABLE_DEBUG_INFOR 

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>//
#include <dxgi1_4.h>//
#include "Rendering/DirectX12/d3dx12.h"//
#include "Rendering/DirectX12/DDSTextureLoader.h"//
#include <D3Dcompiler.h>//
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "Debug/EngineDebug.h"

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine* Engine;

#define THIRD_PARTY_LIBRARY 1