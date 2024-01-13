#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Enigne/Core/RenderingEngine.h"

class CDirectXRenderingEngine;
class CWorld;
class CMeshManage;
class CWindowsEngine : public CEngine
{
	friend class IDirectXDeviceInterface;
public:
	CWindowsEngine();
	
	~CWindowsEngine() override;

	int PreInit(FWinMainCommandParameters InParameters) override;

	int Init(FWinMainCommandParameters InParameters) override;
	
	int PostInit() override;

	void Tick(float DeltaTime) override;

	int PreExit() override;
	
	int Exit() override;
	
	int PostExit() override;

	CDirectXRenderingEngine* GetRenderingEngine() const { return RenderingEngine; }
	
	CMeshManage* GetMeshManage() const;
	
	CWorld* GetWorld() const { return World; }

	bool InitWindows(FWinMainCommandParameters InParameters);

protected:
	HWND MainWindowsHandle;//Ö÷windows¾ä±ú
	
	CDirectXRenderingEngine* RenderingEngine;
	
	CWorld* World;
};
#endif