#pragma once

namespace pkodev
{
	namespace address
	{
        // GameServer.exe 1.36
        namespace GAMESERVER_136
        {
            // const char* g_GetJobName(short sJobID)
            const unsigned int g_GetJobName = 0x00457190;

            // short g_GetJobID(const char *szJobName)
            const unsigned int g_GetJobID = 0x0047FA90;
        }

        // GameServer.exe 1.38
        namespace GAMESERVER_138
        {
            // const char* g_GetJobName(short sJobID)
            const unsigned int g_GetJobName = 0x00459C40;

            // short g_GetJobID(const char *szJobName)
            const unsigned int g_GetJobID = 0x00483870;
        }
	}
}