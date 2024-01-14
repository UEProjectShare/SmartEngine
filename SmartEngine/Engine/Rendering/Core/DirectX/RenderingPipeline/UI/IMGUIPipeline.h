#pragma once

#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FIMGUIPipeline : public IDirectXDeviceInterface_Struct
{
	FIMGUIPipeline();

	void Init(ID3D12DescriptorHeap* InHeap, UINT InOffset);

	void Draw(float DeltaTime);

	void Exit();

protected:
	void Tick(float DeltaTime);
};