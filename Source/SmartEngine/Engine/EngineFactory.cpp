#include "EngineFactory.h"
#if defined(_WIN32)
#include "Platform/Windows/WindowsEngine.h"
#endif

FEngineFactory::FEngineFactory()
{

}

CEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
	return new CWindowsEngine();
#elif defined(__linux__)

#endif // DEBUG
}
