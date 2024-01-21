#pragma once

//#include "../../EngineMinimal.h"
#include "../../EngineMacro.h"

//#ifndef EDITOR_ENGINE
//#define EDITOR_ENGINE 1
//#endif // !EDITOR_ENGINE

#if EDITOR_ENGINE
#include "../../../EditorEngine/LogEditor/LogObject/LogSystem.h"
#endif

#if defined(_WIN32)
#include "simple_library/public/simple_library.h"

#if EDITOR_ENGINE
#define Engine_Log(format,...)\
log_log(format,__VA_ARGS__)\
FEditorLogSystem::Get()->AddLog(format,__VA_ARGS__);
#else
#define Engine_Log(format,...)\
log_log(format,__VA_ARGS__)
#endif

#if EDITOR_ENGINE
#define Engine_Log_Success(format,...)\
log_success(format,__VA_ARGS__)\
FEditorLogSystem::Get()->AddSuccess(format,__VA_ARGS__);
#else
#define Engine_Log_Success(format,...)\
log_success(format,__VA_ARGS__)
#endif

#if EDITOR_ENGINE
#define Engine_Log_Error(format,...)\
log_error(format,__VA_ARGS__)\
FEditorLogSystem::Get()->AddError(format,__VA_ARGS__);
#else
#define Engine_Log_Error(format,...)\
log_error(format,__VA_ARGS__)
#endif

#if EDITOR_ENGINE
#define Engine_Log_Warning(format,...)\
log_warning(format,__VA_ARGS__)\
FEditorLogSystem::Get()->AddWarning(format,__VA_ARGS__);
#else
#define Engine_Log_Warning(format,...)\
log_warning(format,__VA_ARGS__)
#endif

#elif 0

#define Engine_log(format,...) 
#define Engine_Log_Success(format,...) 
#define Engine_Log_Error(format,...) 
#define Engine_Log_Warning(format,...) 

#endif