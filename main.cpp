#include <windows.h>
#include <psapi.h>
#include "nya_commonhooklib.h"

auto module = (uintptr_t)GetModuleHandleA(nullptr);

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			MODULEINFO info;
			GetModuleInformation(GetCurrentProcess(), (HMODULE)module, &info, sizeof(info));
			if ((uintptr_t)info.EntryPoint - (uintptr_t)info.lpBaseOfDll != 0x3C4040) {
				MessageBoxA(nullptr, "Unsupported game version! Make sure you're using v1.3 (.exe size of 6029312 bytes)", "nya?!~", MB_ICONERROR);
				return TRUE;
			}

			// skip Attrib::Instance::Add in GRaceCustom::SetAttribute
			// this'll make the game set the base attrib value, i.e. the actual parent race properties rather than the custom race
			// not sure if this is exactly how lap glitch works in 1.2, but it achieves the same effect :3
			NyaHookLib::PatchRelative(NyaHookLib::CALL, module + 0x1E74E6, module + 0x59048);
		} break;
		default:
			break;
	}
	return TRUE;
}