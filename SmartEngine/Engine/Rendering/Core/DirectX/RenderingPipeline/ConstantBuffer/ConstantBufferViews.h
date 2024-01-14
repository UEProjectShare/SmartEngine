#pragma once
#include "../../../../../Interface\DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

struct FConstantBufferViews : public IDirectXDeviceInterface_Struct
{
	void CreateConstant(UINT ObjectSize, UINT ObjectCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData) const;

	void BuildConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,
		UINT InConstantBufferNum,
		UINT InHandleOffset = 0);

	UINT GetConstantBufferByteSize() const;

	ID3D12Resource* GetBuffer() const { return Constant->GetBuffer(); }
protected:
	shared_ptr<FRenderingResourcesUpdate> Constant;
};