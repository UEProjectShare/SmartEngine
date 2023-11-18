#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

int Init(FEngine* InEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters(hInstance, prevInstance, cmdLine, showCmd)
#endif 
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Init();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

void Tick(FEngine* InEngine)
{
	InEngine->Tick();
}

int Exit(FEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

//hInstance 自己的实例
//prevInstance 上次的实例
//cmdLine 传递命令
//showCmd 多少条cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		//初始化
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

		//渲染出图
		while (true)
		{
			Tick(Engine);
		}

		//退出
		ReturnValue = Exit(Engine);
	}
	else
	{
		ReturnValue = 1;
	}

	Engine_Log("[%i]The engine has exited.", ReturnValue);
	return ReturnValue;
}