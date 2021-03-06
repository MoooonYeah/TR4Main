#pragma once

#ifdef DLL_INJECT
#define DrawFire ((void(__cdecl*)(BYTE size, short roomNumber)) 0x00486DD0)
#define DrawHorizon ((void(__cdecl*)(short *ptr)) 0x0047F620)
#define DrawHorizonLayer ((void(__cdecl*)(unsigned short color1, unsigned short color2, int horizon_height, int flags)) 0x00488950)
#define CalcHorizonAndLayer ((void(__cdecl*)(void)) 0x00489480)
#define SetGunFlash ((void(__cdecl*)(int weapon_type)) 0x00439B80)
#define SetupGunFlash ((void(__cdecl*)(short *pos)) 0x00439AE0)
#define DrawGunFlashes ((void(__cdecl*)(void)) 0x00439C00)
#define InitialiseFogBulbs ((void(__cdecl*)(void)) 0x00481DF0)
#define CreateFXBulbs ((void(__cdecl*)(void)) 0x004819B0)
#define InitDynamicLighting ((void(__cdecl*)(void)) 0x004790F0)
#define DrawTrainEffect ((void(__cdecl*)(void)) 0x00464030)
#define DrawGunShells ((void(__cdecl*)(void)) 0x004395B0)
#define DrawRope ((void(__cdecl*)(void)) 0x00459410)
#define DrawSparks ((void(__cdecl*)(void)) 0x0043A1B0)
#define DrawWeather ((void(__cdecl*)(void)) 0x0048A840)
#define DrawFires ((void(__cdecl*)(void)) 0x00438560)
#define DrawSmoke ((void(__cdecl*)(void)) 0x00487260)
#define DrawSplashes ((void(__cdecl*)(void)) 0x00486430)
#define DrawBubbles ((void(__cdecl*)(void)) 0x004860D0)
#define DrawDebris ((void(__cdecl*)(void)) 0x00489B90) // shatter effect
#define DrawBlood ((void(__cdecl*)(void)) 0x0048A0C0)
#define DrawDrips ((void(__cdecl*)(void)) 0x0048A430)
#define DrawShockwaves ((void(__cdecl*)(void)) 0x0048CAA0)
#define DrawLightning ((void(__cdecl*)(void)) 0x0040E630)
#define DrawLaserSightSprite ((void(__cdecl*)(void)) 0x00489950)
#define DrawMotorBikeSpeedoMeter ((void(__cdecl*)(int screenX, int screenY, int motorBikeSpeed, int angleMin, int angleMax, int speedMax)) 0x0048C6C0)
#endif