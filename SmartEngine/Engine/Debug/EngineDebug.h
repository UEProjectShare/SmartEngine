#pragma once

#define ANALYSIS_HRESULT(InValue) \
{\
	if (FAILED(InValue))\
	{\
		Engine_Log_Error("Error = %i", (int)InValue);\
		assert(0);\
	}\
	else if (SUCCEEDED(InValue))\
	{\
		Engine_Log_Success("Success !");\
	}\
}