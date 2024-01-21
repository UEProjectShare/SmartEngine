#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Core/World.h"
#include "../../Core/Camera.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Rendering/Enigne/DirectX/DirectX12RenderingEngine.h"

#if EDITOR_ENGINE
#include "../../../EditorEngine/EditorEngine.h"
#endif

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CWindowsEngine::CWindowsEngine()
	: RenderingEngine(new CDirectX12RenderingEngine())
#if EDITOR_ENGINE
	, EditorEngine(new CEditorEngine())
#endif // 0
{
	
}

CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	InitPath();

	//��־ϵͳ��ʼ��
	std::string LogPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineLogsPath());

	init_log_system(LogPath.c_str());
	Engine_Log("Log Init.");

	//��������


	Engine_Log("Engine pre initialization complete.");

	//��Ⱦ�����ʼ��
	RenderingEngine->PreInit(InParameters);

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	InitWindows(InParameters);

	RenderingEngine->SetMainWindowsHandle(MainWindowsHandle);

	RenderingEngine->Init(InParameters);

	//ע���Ӧ��world
	FCreateObjectParam Param;
	Param.Outer = this;
	World = CreateObject<CWorld>(Param, new CWorld());
	RenderingEngine->World = World;

	Engine_Log("Engine initialization complete.");

	return 0;
}

int CWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");
	for (const auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}
	RenderingEngine->PostInit();


	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	for (const auto& Tmp : GObjects)
	{
		if (Tmp->IsTick())
		{
			Tmp->Tick(DeltaTime);
		}
	}

	if (World)
	{
		if (World->GetCamera())
		{
			FViewportInfo ViewportInfo;

			const XMFLOAT3 ViewPosition = World->GetCamera()->GetPosition();
			ViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);
			
			ViewportInfo.ViewMatrix = World->GetCamera()->ViewMatrix;
			
			ViewportInfo.ProjectMatrix = World->GetCamera()->ProjectMatrix;
			
			RenderingEngine->UpdateCalculations(DeltaTime, ViewportInfo);

			RenderingEngine->Tick(DeltaTime);
		}
	}
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine post exit complete.");
	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();


	Engine_Log("Engine post exit complete.");
	return 0;
}

void CWindowsEngine::InitPath()
{
	auto CreateFileDirectory = [](const std::string& InPath)
		{
			const std::string LogPath =
				FEnginePathHelper::RelativeToAbsolutePath(
					InPath);
			//����·��
			create_file_directory(LogPath.c_str());
		};

	//��־ϵͳ��ʼ��
	CreateFileDirectory(FEnginePathHelper::GetEngineLogsPath());

	//Content
	CreateFileDirectory(FEnginePathHelper::GetEngineContentPath());

	const wstring ShadersPathW = FEnginePathHelper::GetEngineShadersPath();
	char ShaderPath[1024] = { 0 };
	wchar_t_to_char(ShaderPath, 1024, ShadersPathW.c_str());
	CreateFileDirectory(ShaderPath);

	CreateFileDirectory(FEnginePathHelper::GetEngineBinariesPath());
}

CMeshManage* CWindowsEngine::GetMeshManage() const
{
	return RenderingEngine->GetMeshManage();
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	//ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr;//����������ľ���GDI����
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);//����һ����ͷ���
	WindowsClass.hIcon = nullptr; //Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = nullptr;//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance; //����ʵ��
	WindowsClass.lpszClassName = L"SmartEngine";//��������
	WindowsClass.lpszMenuName = nullptr;//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//��ô���ƴ��� ��ֱ��ˮƽ�ػ�
	WindowsClass.lpfnWndProc = EngineWindowProc;//��Ϣ������

	//ע�ᴰ��
	const ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register windows class fail.");
		MessageBox(nullptr, L"Register windows class fail.", L"Error", MB_OK);
	}

	RECT Rect = {0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight};
	
	//@rect �ʿ�
	//WS_OVERLAPPEDWINDOW �ʿڷ��
	//NULL û�в˵�
	AdjustWindowRect(&Rect,WS_OVERLAPPEDWINDOW, NULL);

	const int WindowWidth = Rect.right - Rect.left;
	const int WindowHeight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,//���ڶ���ķ��
		L"SmartEngine", // ��������
		L"SmartEngine",//����ʾ�ڴ��ڵı�������ȥ
		WS_OVERLAPPEDWINDOW, //���ڷ��
		100, 100,//���ڵ�����
		WindowWidth, WindowHeight,//
		nullptr, //�����ھ��
		nullptr, //�˵����
		InParameters.HInstance,//����ʵ��
		nullptr);//
	
	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//��ʾ����
	ShowWindow(MainWindowsHandle, SW_SHOW);

	//��������ģ�ˢ��һ��
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");

	return true;
}

#endif