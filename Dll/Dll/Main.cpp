#include <stdio.h>
#include <Windows.h>
#include <string>

#include "Utils.h"
#include "VMTHook.h"

/* o_function_a and o_function_b will be used to re-call the original functions. */
typedef void(__cdecl* func)();
func o_function_a = nullptr;
func o_function_b = nullptr;

/* function_hooked_a will repalce the function_a in the vtable. */
void function_hooked_a()
{
	printf("::function_hooked_a()\n");
	// calling the original function after our code has executed to avoid errors and crashes.
	o_function_a();
}

/* function_hooked_b will replace the function_b in the vtable. */
void function_hooked_b()
{
	printf("::function_hooked_b()\n");
	// calling the original function after our code has executed to avoid errors and crashes.
	o_function_b();
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	// getting the address of the vtable
	void** vfunctable = *(void***)(utils::pattern_scan(GetModuleHandleA("Exe.exe"), "C7 06 ? ? ? ? 8B 06") + 2);

	/* preparing the hooking and doing the hooking. */
	vmthook hook(vfunctable);
	hook.hook(0, function_hooked_a);
	hook.hook(1, function_hooked_b);

	/* assigning o_function_a and o_function_b */
	o_function_a = hook.get_original<func>(0);
	o_function_b = hook.get_original<func>(1);

	return 0;
}

/* main of our dll. */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);
		CreateThread(0, 0, MainThread, hInstance, 0, 0);
	}

	return true;
}