#pragma once

extern void ShiftItem(ITEM_INFO* item, COLL_INFO* coll);

#ifdef DLL_INJECT
#define GetCollisionInfo ((void(__cdecl*)(COLL_INFO* coll, int x, int y, int z, short roomNumber, int height)) 0x00446170)
#define CreatureCollision COLLISION(0x00447470)
#define ObjectCollision COLLISION(0x004475F0)
#define MovBlockCollision COLLISION(0x00448840)
#define CogCollision COLLISION(0x00448CC0)
#endif