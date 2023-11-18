#include "WindowsEngine.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//日志系统初始化
	const char LogPath[] = "Log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//处理命令

	//

	if (InitWindows(InParameters))
	{

	}

	Engine_Log("Engine pre initialization complete.");
	return 0;
}

int FWindowsEngine::Init()
{
	Engine_Log("Engine initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");
	return 0;
}

void FWindowsEngine::Tick()
{

}

int FWindowsEngine::PreExit()
{
	Engine_Log("Engine post exit complete.");
	return 0;
}

int FWindowsEngine::Exit()
{
	Engine_Log("Engine exit complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	Engine_Log("Engine post exit complete.");
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	//注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;//是否需要额外空间
	WindowsClass.cbWndExtra = 0;//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;//如果有设置哪就是GDI擦除
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);//设置一个箭头光标
	WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = nullptr;//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance; //窗口实例
	WindowsClass.lpszClassName = L"SmartEngine";//窗口名字
	WindowsClass.lpszMenuName = nullptr;//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 垂直和水平重绘
	WindowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

	//注册窗口
	const ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register windows class fail.");
		MessageBox(nullptr, L"Register windows class fail.", L"Error", MB_OK);
	}

	RECT Rect = {0, 0, 1280, 720};
	
	//@rect 适口
	//WS_OVERLAPPEDWINDOW 适口风格
	//NULL 没有菜单
	AdjustWindowRect(&Rect,WS_OVERLAPPEDWINDOW, NULL);

	const int WindowWidth = Rect.right - Rect.left;
	const int WindowHeight = Rect.bottom - Rect.top;

	const HWND Hwnd = CreateWindowEx(
		NULL,//窗口额外的风格
		L"SmartEngine", // 窗口名称
		L"Smart Engine",//会显示在窗口的标题栏上去
		WS_OVERLAPPEDWINDOW, //窗口风格
		100, 100,//窗口的坐标
		WindowWidth, WindowHeight,//
		nullptr, //父窗口句柄
		nullptr, //菜单句柄
		InParameters.HInstance,//窗口实例
		nullptr);//

	if (!Hwnd)
	{
		Engine_Log_Error("CreateWindow Failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//显示窗口
	ShowWindow(Hwnd, SW_SHOW);

	//窗口是脏的，刷新一下
	UpdateWindow(Hwnd);

	Engine_Log("InitWindows complete.");

	return true;
}

#endif