#pragma once
#include "../Engine/EngineMacro.h"
#include "CoreMacro.h"

#if EDITOR_ENGINE
class GActorObject;

class FOperationHandleSelectManage
{
public:
	FOperationHandleSelectManage();

	static FOperationHandleSelectManage* Get();
	
	static void Destroy();

	bool IsCaptureMouseNotOnUI();

	void AllOperationHandleHide();
	
	void DisplaySelectedOperationHandle(GActorObject* InNewSelectedObject);
	
	void DisplaySelectedOperationHandle();
	
	void HideSelectedOperationHandle() const;
	
	//��ʾ�µ��ֱ�
	void SetNewSelectedOperationHandle(GActorObject* InNewSelectedObject);
	
	//����ѡ�����
	void SetNewSelectedObject(GActorObject* InNewSelectedObject);

	GActorObject* GetSelectedOperationHandle() const;
private:
	static FOperationHandleSelectManage* InManage;

	//��¼��ǰ�Ĳ����ֱ� �����´�����ʾ����
	GActorObject* SelectedOperationHandle;
};

#endif