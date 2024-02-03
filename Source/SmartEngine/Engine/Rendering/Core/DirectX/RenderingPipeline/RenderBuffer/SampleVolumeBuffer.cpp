#include "SampleVolumeBuffer.h"

FSampleVolumeBuffer::FSampleVolumeBuffer()
{
	BuildRandomVolumeBuffer();
}

void FSampleVolumeBuffer::BuildVolumeBuffer()
{
	VolumeBuffer.clear();

	//构造八个角
	VolumeBuffer.push_back(fvector_4d(+1.0f, +1.0f, +1.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(-1.0f, -1.0f, -1.0f, 0.0f));
																
	VolumeBuffer.push_back(fvector_4d(-1.0f, +1.0f, +1.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(+1.0f, -1.0f, -1.0f, 0.0f));
																
	VolumeBuffer.push_back(fvector_4d(+1.0f, +1.0f, -1.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(-1.0f, -1.0f, +1.0f, 0.0f));
																
	VolumeBuffer.push_back(fvector_4d(-1.0f, +1.0f, -1.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(+1.0f, -1.0f, +1.0f, 0.0f));

	//构建中心点
	VolumeBuffer.push_back(fvector_4d(-1.0f, 0.0f, 0.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(+1.0f, 0.0f, 0.0f, 0.0f));

	VolumeBuffer.push_back(fvector_4d(0.0f, -1.0f, 0.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(0.0f, +1.0f, 0.0f, 0.0f));

	VolumeBuffer.push_back(fvector_4d(0.0f, 0.0f, -1.0f, 0.0f));
	VolumeBuffer.push_back(fvector_4d(0.0f, 0.0f, +1.0f, 0.0f));
}

void FSampleVolumeBuffer::BuildRandomVolumeBuffer()
{
	if (VolumeBuffer.size() == 0)
	{
		BuildVolumeBuffer();
	}

	for (auto& Tmp : VolumeBuffer)
	{
		Tmp.normalize();

		Tmp *= get_float_random_in_range(0.24f, 1.f);
	}
}

void FSampleVolumeBuffer::Update(XMFLOAT4* InBuffer)
{
	memcpy(InBuffer, GetVolumeBuffer(), VolumeBuffer.size() * sizeof(fvector_4d));
}
