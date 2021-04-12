/*
 * THIS WAS CREATED USING VISUAL STUDIO COMMUNITY EDITION 2019 - V 16.9.0
 */

 /*
  * This is the main point of entry for the program and handles
  *		- Platfrom initialisation.
  *		- Game creation.
  * 
  * Original @author Abertay University
  *
  * Updated by @author D. Green.
  *
  * © D. Green. 2021.
  */

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <platform/d3d11/system/platform_d3d11.h>
#include "Game.h"
#include <time.h>
#include <stdlib.h> 

/////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int sceLibcHeapSize = 128*1024*1024;	// Sets up the heap area size as 128MiB.

/////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	/* initialize random seed: */
	srand(time(NULL));

	// initialisation
	gef::PlatformD3D11 platform(hInstance, 1440, 810, false, true);

	Game epicAbe(platform);
	epicAbe.Run();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////