#pragma once

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>//com对象组件 comPtr<> 
#include <dxgi1_4.h>//dxgi 
#include <d3d12.h>//
#include <D3Dcompiler.h>//
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace Microsoft::WRL;