#pragma once
#include "../../Core/CoreObject/GuidInterface.h"
#include "../../Core/Engine.h"
#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif
//�ṩ��Ⱦ���ݵĽӿ�
class IRenderingInterface //:public IGuidInterface
{
	friend class CDirectXRenderingEngine;
public:
	IRenderingInterface();
	
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);
//		
//private:
//	static vector<IRenderingInterface*> RenderingInterface;
};
