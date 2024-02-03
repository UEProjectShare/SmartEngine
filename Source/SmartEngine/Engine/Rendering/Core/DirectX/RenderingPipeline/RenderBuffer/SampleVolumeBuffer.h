#pragma once
#include "Core/RenderBuffer.h"

struct FSampleVolumeBuffer
{
	FSampleVolumeBuffer();

public:
	void BuildVolumeBuffer();
	
	void BuildRandomVolumeBuffer();

	void Update(XMFLOAT4 *InBuffer);
	
	void* GetVolumeBuffer() {return VolumeBuffer.data();}

protected:
	std::vector<fvector_4d> VolumeBuffer;
};