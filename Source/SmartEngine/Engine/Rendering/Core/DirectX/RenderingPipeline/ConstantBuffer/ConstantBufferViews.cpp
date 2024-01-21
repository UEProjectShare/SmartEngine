#include "ConstantBufferViews.h"

void FConstantBufferViews::CreateConstant(UINT ObjectSize, UINT ObjectCount, bool bConstBuffer)
{
	Constant = make_shared<FRenderingResourcesUpdate>();
	Constant->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount, bConstBuffer);
}

void FConstantBufferViews::Update(int Index, const void* InData) const
{
    Constant->Update(Index, InData);
}

void FConstantBufferViews::BuildConstantBuffer(
    CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,
    UINT InConstantBufferNum,
    UINT InHandleOffset)
{
    const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    const D3D12_GPU_VIRTUAL_ADDRESS Addr = Constant->GetBuffer()->GetGPUVirtualAddress();

    for(UINT i = 0; i < InConstantBufferNum; i++)
    {
        //每一帧都是起始地址
        CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = InHandle;

        D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
        CBVDesc.BufferLocation = Addr + i * Constant->GetConstantBufferByteSize();
        CBVDesc.SizeInBytes = Constant->GetConstantBufferByteSize();

        Handle.Offset(i + InHandleOffset, DescriptorOffset);

        GetD3dDevice()->CreateConstantBufferView(&CBVDesc, Handle);
    }
}

UINT FConstantBufferViews::GetConstantBufferByteSize() const
{
    return Constant->GetConstantBufferByteSize();
}
