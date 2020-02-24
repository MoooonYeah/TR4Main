#pragma once
#include "specific/init.h"

// thank to GhostBlade for Macro and Read():
// it's compatible with Tomb4 since it's the same level loader.
// https://github.com/TOMB5/TOMB5/blob/9707b4ec30b374d095af499c93780daeb3edb5f5/SPEC_PC_N/FILE.CPP#L90

extern int8_t ReadSByte();
extern uint8_t ReadUByte();
extern int16_t ReadSWord();
extern uint16_t ReadUWord();
extern int32_t ReadSDword();
extern uint32_t ReadUDword();
extern uint16_t FReadWord();
extern uint32_t FReadDword();
extern void ReadBytes(LPVOID dst, uint32_t count);
extern void FReadBytes(LPVOID dst, uint32_t count);

#define OnlyReadT(d, s, n) ReadBytes(d, sizeof(s) * n)
#define OnlyReadTB(d, s, n) ReadBytes(d + 1, sizeof(s) * n)
#define OnlyReadTF(d, s, n) FReadBytes(d, sizeof(s) * n)
#define OnlyRead(d, s, n) ReadBytes((d), sizeof(struct s) * n)
#define OnlyReadB(d, s, n) ReadBytes((d) + 1, sizeof(struct s) * n)
#define OnlyReadF(d, s, n) FReadBytes((d), sizeof(struct s) * n)

#define AllocT(d, s, n) d = (s*)game_malloc(sizeof(s) * n)
#define AllocReadT(d, s, n) AllocT((d), s, n);OnlyReadT(d, s, n)
#define AllocReadTB(d, s, n) AllocT(d, s, n);OnlyReadT(d + 1, s, n)
#define AllocReadTBS(d, s, n) AllocT(d, s, n + 2);OnlyReadT(d + 1, s, n)
#define AllocReadTF(d, s, n) AllocT((d), s, n);OnlyReadTF(d, s, n)
#define Alloc(d, s, n) d = (struct s*)game_malloc(sizeof(struct s) * n)
#define AllocRead(d, s, n) Alloc((d), s, n);OnlyRead(d, s, n)
#define AllocReadB(d, s, n) Alloc((d), s, n);OnlyRead(d + 1, s, n)
#define AllocReadF(d, s, n) Alloc((d), s, n);OnlyReadF(d, s, n)

#define AddPtr(p, t, n) p = (t*)((char*)(p) + (ptrdiff_t)(n));