#include <platform/d3d11/system/platform_d3d11.h>
#include "Game.h"

unsigned int sceLibcHeapSize = 128*1024*1024;	// Sets up the heap area size as 128MiB.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// initialisation
	gef::PlatformD3D11 platform(hInstance, 2000, 1125, false, true);

	Game epicAbe(platform);
	epicAbe.Run();

	return 0;
}