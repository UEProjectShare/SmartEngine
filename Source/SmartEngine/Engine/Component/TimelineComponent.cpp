#include "TimelineComponent.h"

FTimeline::FTimeline()
	: TimeLength(0.f)
	, CurrentTimeLength(0.f)
{
}

void FTimeline::Tick(float DeltaTime)
{
	CurrentTimeLength -= DeltaTime;
	if (CurrentTimeLength > 0.f)
	{
		if (TimelineDelegate.IsBound())
		{
			TimelineDelegate.Execute(TimeLength, DeltaTime);
		}
	}
	else
	{
		CurrentTimeLength = 0.f;
		TimelineDelegate.ReleaseDelegate();
	}
}

void FTimeline::BindTimeLineEvent(float InTime, const FTimelineDelegate& InTimelineDelegate)
{
	TimeLength = InTime;
	CurrentTimeLength = InTime;
	TimelineDelegate = InTimelineDelegate;
}
