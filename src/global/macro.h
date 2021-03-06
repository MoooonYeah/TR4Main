#pragma once

#ifdef DLL_INJECT
#define Log ((int(__cdecl*)(char* LT_flags, LPCSTR format, ...)) 0x0049C750)
#define WinDisplayString ((void(__cdecl*)(int x, int y, LPCSTR message, ...)) 0x0048F8C0)
#endif

#define BOX_DEFFLAG(icon) (MB_OK | icon | MB_SETFOREGROUND | MB_SYSTEMMODAL) // set foreground, only ok button and with a icon.
#define OCB_MODE(x) ((x) % 1000)
#define SQUARE(x) ((x)*(x))
#define CLAMP(x, a, b) ((x)<(a)?(a):((x)>(b)?(b):(x)))
#define SIGN(x) ((0 < (x)) - ((x) < 0))
#define SWAP(a,b,c) { (c)=(a); (a)=(b); (b)=(c); }
#define CLAMPADD(x, a, b) ((x)<(a)?((x)+(a)):((x)>(b)?((x)-(b)):0))
#define ENLARGEDOWN(a, b) if (a < b) b = a
#define ENLARGEUP(a, b) if (a > b) b = a
#define ENLARGEDOWN2(a, b) if (a < b) b = (short)a
#define ENLARGEUP2(a, b) if (a > b) b = (short)a
#define CLIPDOWN(a, b) if (a < b) a = b
#define CLIPUP(a, b) if (a > b) a = b
#define XZ_GET_SECTOR(room, xPos, zPos) (room->floor[((zPos - room->z) >> WALL_SHIFT) + ((xPos - room->x) >> WALL_SHIFT) * room->x_size])
#define memcpy_long(dest, src, size) memcpy(dest, src, size * 4)

#define SIN(x) (4 * rcossin_tbl[(x >> 3) & ANGLE(45)])
#define COS(x) (4 * rcossin_tbl[((x >> 3) & ANGLE(45)) + 1])
#define SINN(x)    (rcossin_tbl[(x >> 3) & ANGLE(45)])       // not multiplied by 4
#define COSN(x)    (rcossin_tbl[((x >> 3) & ANGLE(45)) + 1]) // not multiplied by 4

#define BONE1(id) *(bone + (1 + (id * 4)))
#define BONE2(id) *(bone + (2 + (id * 4)))
#define BONE3(id) *(bone + (3 + (id * 4)))
#define MESH(object, x) (object->mesh_index + x * 2)

#define ABS(x) (((x)<0) ? (-(x)):(x))
#define MTB(x) (x - 1) // mesh to bone

#define CustABS(A, B)\
{\
if (A > B)\
    A = B; \
else if (A < -B)\
    A = -B; \
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef MIN
#define MIN(x, y) (((x)<(y))?(x):(y))
#endif

#ifndef MAX
#define MAX(x, y) (((x)>(y))?(x):(y))
#endif

#ifndef SIGN
#define SIGN(x) ((0 < (x)) - ((x) < 0))
#endif

#define SECTOR(x) ((x) * WALL_L)
#define CLICK(x) ((x) * STEP_L)
#define STEP(x) ((x) * STEP_L / 2)

#define TRIGMULT2(a, b) (((a) * (b)) >> W2V_SHIFT)
#define TRIGMULT2F(a, b) (((a) * (b)) / 16384.0f)
#define TRIGMULT3(a, b, c) TRIGMULT2(TRIGMULT2(a, b), c)

#define CHK_ANY(x, y) (x & y) != 0
#define CHK_EXI(x, y) x & y
#define CHK_EQA(x, y) (x & y) != y
#define CHK_NOP(x, y) !(x & y)

#define INITIALISE(add) ((void(__cdecl*)(short itemNumber)) add)
#define COLLISION(add) ((void(__cdecl*)(short itemNumber, ITEM_INFO* laraitem, COLL_INFO* coll)) add)
#define CONTROL(add) ((void(__cdecl*)(short itemNumber)) add)
#define DRAW(add) ((void(__cdecl*)(ITEM_INFO* item)) add)
#define FLOOR(add) ((void(__cdecl*)(ITEM_INFO* item, int x, int y, int z, int* height)) add)
#define CEILING(add) ((void(__cdecl*)(ITEM_INFO* item, int x, int y, int z, int* height)) add)

#define RWATER(roomNumber) Rooms[roomNumber].flags & ROOM_WATER
#define RNOT_WATER(roomNumber) !(Rooms[roomNumber].flags & ROOM_WATER)
#define AI(item, ai_flags) item->ai_bits & ai_flags
#define NOT_AI(item, ai_flags) !(item->ai_bits & ai_flags)

#define CHECK_REACHED_GOAL(item, enemy, range) abs(enemy->pos.x - item->pos.x) < range && abs(enemy->pos.z - item->pos.z) < range &&  abs(enemy->pos.y - item->pos.y) < range

#define INIT_PICKUP(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = PickupControl;\
    obj->initialise = InitialisePickup;\
    obj->collision = PickupCollision;\
    obj->saveFlags = true;\
    obj->savePosition = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_KEY_HOLE(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->collision = KeyHoleCollision;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_PUZZLE_HOLE(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = PuzzleControl;\
    obj->collision = PuzzleHoleCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_PUZZLE_DONE(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = PuzzleControl;\
    obj->collision = PuzzleDoneCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_ANIMATING(slotID, haveCollision)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = PuzzleControl;\
    if (haveCollision)\
        obj->collision = ObjectCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_DOOR(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->initialise = InitialiseDoor;\
    obj->control = DoorControl;\
    obj->collision = DoorCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_SWITCH(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = SwitchControl;\
    obj->collision = SwitchCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define INIT_SMASH_OBJECT(slotID)\
obj = &Objects[slotID];\
if (obj->loaded)\
{\
    obj->control = SwitchControl;\
    obj->collision = SwitchCollision;\
    obj->saveAnim = true;\
    obj->saveFlags = true;\
    obj->hitEffect = HIT_FRAGMENT;\
}

#define GET_DD(a) App.DeviceInfo.DDInfo[a]
#define GET_DS(a) App.DeviceInfo.DSInfo[a]
#define GET_D3D(a) GET_DD(App.DXConfig.nDD).D3DInfo[a]
#define GET_DM(a) GET_D3D(App.DXConfig.nD3D).DisplayMode[a]
#define GET_TEX(a) GET_D3D(App.DXConfig.nD3D).Texture[a]

#define CUR_DD GET_DD(App.DXConfig.nDD)
#define CUR_DS GET_DS(App.DXConfig.nSoundCard)
#define CUR_D3D GET_D3D(App.DXConfig.nD3D)
#define CUR_DM GET_DM(App.DXConfig.nVMode)
#define CUR_TEX GET_TEX(App.DXConfig.nTexture)