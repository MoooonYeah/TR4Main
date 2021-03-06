#pragma once

#define NO_CAMERA -1
#define FOLLOW_CENTRE 1
#define NO_CHUNKY 2
#define CHASE_OBJECT 3

#ifdef DLL_INJECT
#define bUseSpotCam     VAR_I_(0x004BF6EC, int, 0)

#define InitialiseCamera ((void(__cdecl*)(void)) 0x00442E70)
#define InitSpotCamSequences ((void(__cdecl*)(void)) 0x004602B0)

#define LookCamera ((void(__cdecl*)(void)) 0x00443ED0)
#define LookUpDown ((void(__cdecl*)(void)) 0x00428C40)
#define LookLeftRight ((void(__cdecl*)(void)) 0x00428D40)
#define ResetLook ((void(__cdecl*)(void)) 0x00428BA0)
#endif