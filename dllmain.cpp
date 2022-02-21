#include <Windows.h>

#include <iostream>
#include <cstring>

#include "loader.h"
#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"

namespace pkodev
{
	#ifdef MOD_LANG_RUS

		// Job names
		const char* szJobNames[] = 
		{
				"Новичок",
				"Мечник",
				"Охотник",
				"Моряк",
				"Мореплаватель",
				"Знахарка",
				"Ремесленник",
				"Торговец",
				"Чемпион",
				"Воитель",
				"Белый Рыцарь",
				"Укротитель Животных",
				"Стрелок",
				"Целительница",
				"Колдунья",
				"Капитан",
				"Покоритель морей",
				"Выскочка",
				"Инженер"
		};

	#else

		// Job names
		const char* szJobNames[] = 
		{
				"Newbie",
				"Swordsman",
				"Hunter",
				"Sailor",
				"Explorer",
				"Herbalist",
				"Artisan",
				"Merchant",
				"Champion",
				"Crusader",
				"White Knight",
				"Animal Tamer",
				"Sharpshooter",
				"Cleric",
				"Seal Master",
				"Captain",
				"Voyager",
				"Upstart",
				"Engineer"
		};

	#endif

	// Set function hook
	void set_func_hook(unsigned int address, unsigned int func);
}


// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
	// Make some hooks
	{
		// g_GetJobName()
		pkodev::set_func_hook(
			pkodev::address::MOD_EXE_VERSION::g_GetJobName,
			reinterpret_cast<unsigned int>(&pkodev::hook::g_GetJobName)
		);

		// g_GetJobID()
		pkodev::set_func_hook(
			pkodev::address::MOD_EXE_VERSION::g_GetJobID,
			reinterpret_cast<unsigned int>(&pkodev::hook::g_GetJobID)
		);
	}

	// Write a log
	std::cout << "[" << TOSTRING(MOD_NAME) << "] Job names fix ver. " 
		<< TOSTRING(MOD_VERSION) << " is applied!" << std::endl;
}

// Stop the mod
void Stop()
{

}

// const char* g_GetJobName(short sJobID)
const char* __cdecl pkodev::hook::g_GetJobName(short int sJobID)
{
	// Number of professions
	const unsigned short n = sizeof(pkodev::szJobNames)
		/ sizeof(pkodev::szJobNames[0]);

	// Look for a profession in the list
	if (sJobID < 0 || sJobID >= n) 
	{
		return pkodev::szJobNames[0];
	}

	return pkodev::szJobNames[sJobID];
}

// short g_GetJobID(const char *szJobName)
short int __cdecl pkodev::hook::g_GetJobID(const char* szJobName)
{
	// Number of professions
	const unsigned short n = sizeof(pkodev::szJobNames)
		/ sizeof(pkodev::szJobNames[0]);

	// Looking for a profession ID in the list
	for (unsigned int i = 0; i < n; ++i)
	{
		if (std::strcmp(pkodev::szJobNames[i], szJobName) == 0)
		{
			return i;
		}
	}

	return 0;
}

// Set function hook
void pkodev::set_func_hook(unsigned int address, unsigned int func)
{
	// Structure of a jump to a function
#pragma pack(push, 1)
	struct jump
	{
		BYTE opcode;
		UINT dif;
		BYTE retn;
	};
#pragma pack(pop)

	// Create a sequence of bytes for
	// jumping to a function:
	//  jmp GetJobName_Hooked
	//  retn
	jump jmp;
	jmp.opcode = 0xE9;
	jmp.dif = (
		func
		-
		address
		-
		5
		);
	jmp.retn = 0xC3;

	// Make memory writable
	DWORD dwOldProt = PAGE_EXECUTE_READWRITE;
	VirtualProtect(
		reinterpret_cast<LPVOID>(address),
		sizeof(jmp),
		dwOldProt,
		&dwOldProt
	);

	// Write jump bytes to GameServer.exe memory
	*reinterpret_cast<jump*>(address) = jmp;

	// Returning memory properties back
	VirtualProtect(
		reinterpret_cast<LPVOID>(address),
		sizeof(jmp),
		dwOldProt,
		&dwOldProt
	);
}