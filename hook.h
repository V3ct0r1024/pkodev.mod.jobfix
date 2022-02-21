#pragma once
#include "structure.h"

namespace pkodev
{
	namespace hook
	{
		// const char* g_GetJobName(short sJobID)
		const char* __cdecl g_GetJobName(short int sJobID);

		// short g_GetJobID(const char *szJobName)
		short int __cdecl g_GetJobID(const char* szJobName);
	}
}