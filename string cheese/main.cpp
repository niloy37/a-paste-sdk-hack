#include "ap_core.h"


BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, void*)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		ap::core::on_attach(instance);
		break;
	case DLL_PROCESS_DETACH:
		ap::core::on_detach();
		break;
	}

	return TRUE;
}