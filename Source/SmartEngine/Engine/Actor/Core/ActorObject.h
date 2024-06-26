#pragma once
#include "../../EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "ActorObject.CodeReflection.h"

class CTransformationComponent;
//G -> Game
class GActorObject : public CCoreMinimalObject
{
	CODEREFLECTION()
	
	CVARIABLE()
	CTransformationComponent* RootComponent;

public:
	GActorObject();
	
	FORCEINLINE CTransformationComponent* GetRootComponent() const {return RootComponent;}
	
	void GetBoundingBox(BoundingBox& OutBoundingBox) const;
	
	BoundingBox GetBoundingBox() const;
	
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	
	virtual void SetRotation(const fvector_3d& InRotation);
	
	virtual void SetRotation(const frotator& InNewRotation);
	
	virtual void SetRotationQuat(const fquat& InNewQuatRotation);
	
	virtual void SetScale(const fvector_3d& InNewScale);

	virtual void SetPickup(bool bNewPickup);
	
	XMFLOAT3& GetPosition() const;
	
	frotator GetRotation() const;
	
	fvector_3d GetScale() const;
	
	fquat GetRotationQuat() const;

	XMFLOAT3& GetForwardVector() const;
	
	XMFLOAT3& GetRightVector() const;
	
	XMFLOAT3& GetUPVector() const;

	//����
	CVARIABLE(CodeType = Resources, Category="VVVV", aaaa = "asdasdas", www = dasdasd, xxxxx, wwww, ddd)
	string NameXXXX;

	CVARIABLE(CodeType = Resources)
	bool PlayerState;

	CVARIABLE(CodeType = Resources)
	fvector_3d posxcxxx;

	CVARIABLE(CodeType = Resources, Step = 4, Draggable)
	int helloInt;

	CVARIABLE(CodeType = Resources, MaxCrop="100", MinCrop=0)
	float PlayerState123;

	CVARIABLE(CodeType = Resources, AllowablePercentage)
	float PlayerState1233;

	CVARIABLE(CodeType = Resources)
	vector<int> TTTT;

	CVARIABLE(CodeType = Resources)
	vector<float> TTTTXXX;
	
	CVARIABLE(CodeType = Resources)
	vector<double> TTTTXXX111;

	CVARIABLE(CodeType = Resources)
	map<int,string> TTTT1234567;

	CVARIABLE(CodeType = Resources)
	map<string,string> TTTT12345671;

	CVARIABLE(CodeType = Resources)
	map<char,long> TTTT12345671long;

	CDIAPER(CodeType = Event)
	static void Hello123(int c, float b);

	CDIAPER(CodeType = Event)
	static void Hello();

	CDIAPER(CodeType = Function)
	static void Hello1(){}

	CDIAPER(CodeType = PureFunction)
	static void Hello2() {}
};