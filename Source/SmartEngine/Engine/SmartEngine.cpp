#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

int Init(CEngine* InEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	const FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif 

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif 
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif 
	);
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

void Tick(CEngine* InEngine)
{
	const float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);

	//Sleep(30);
}

int Exit(CEngine* InEngine)
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

CEngine* Engine = nullptr;

//hInstance 自己的实例
//prevInstance 上次的实例
//cmdLine 传递命令
//showCmd 多少条cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();
	if (Engine)
	{
		//初始化
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

		MSG EngineMsg = { 0 };

		//渲染出图
		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE 消息不从队列里除掉。
			//PM_REMOVE   消息从队列里除掉。
			//PM_NOYIELD  此标志使系统不释放等待调用程序空闲的线程
			// 
			//PM_QS_INPUT 处理鼠标和键盘消息。
			//PM_QS_PAINT 处理画图消息。
			//PM_QS_POSTMESSAGE 处理所有被寄送的消息，包括计时器和热键。
			//PM_QS_SENDMESSAGE 处理所有发送消息。
			if (PeekMessage(&EngineMsg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&EngineMsg);
				DispatchMessage(&EngineMsg);
			}
			else
			{
				Tick(Engine);

				//模拟电脑固定消耗
				Sleep(1);
			}
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