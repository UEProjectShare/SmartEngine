#pragma once
#include "../EngineMinimal.h"

//float �ܳ���
//float ֡ʱ��
DEFINITION_SIMPLE_SINGLE_DELEGATE(FTimelineDelegate, void, float, float);

struct FTimeline
{
	FTimeline();

	void Tick(float DeltaTime);

	void BindTimeLineEvent(float InTime,const FTimelineDelegate& InTimelineDelegate);
private:
	float TimeLength;
	
	float CurrentTimeLength;
	
	FTimelineDelegate TimelineDelegate;
};