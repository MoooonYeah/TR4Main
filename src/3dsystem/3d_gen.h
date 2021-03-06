#pragma once

constexpr float Z_ONE = 2048.0f;
constexpr float MINZR = 0.005f;
constexpr float MAXZR = 0.995f;
constexpr float ZRANGE = (MAXZR - MINZR);
constexpr float W2V_D3D_ASPECT = 0.000061035156f;
constexpr float W2V_ASPECT_CORRECTION = 1.3333334f;

extern void AlterFOV(short newFov);
extern void phd_GenerateW2V(PHD_3DPOS* viewpos);
extern void phd_LookAt(int xsrc, int ysrc, int zsrc, int xtar, int ytar, int ztar, short roll);
extern BOOL phd_TranslateRel(int x, int y, int z);
extern void phd_TranslateAbs(int x, int y, int z);
extern void phd_RotX(short rx);
extern void phd_RotY(short ry);
extern void phd_RotZ(short rz);
extern void phd_RotYXZpack(int angs);
extern void phd_RotYXZ(short ry, short rx, short rz);
extern void phd_GetVectorAngles(int x, int y, int z, short* dest);
extern void ScaleCurrentMatrix(PHD_VECTOR* scale);
extern void SetupZ(int nNear, int nFar);
extern void phd_InitWindow(int x, int y, int width, int height, int nearz, int farz, short viewAngle);
extern void SetupDXMatrixTransformState(void);
extern void SetD3DViewMatrix(void);
extern void InitD3DMatrix(D3DMATRIX *mptr);
extern void SetupDX_W2VMatrix(D3DMATRIX* dest, PHD_MATRIX* pptr);

#ifdef DLL_INJECT
#define dx_mxworld          VAR_U_(0x00753BE0, D3DMATRIX) // D3DTRANSFORMSTATE_WORLD
#define dx_mxprojection     VAR_U_(0x00753918, D3DMATRIX) // D3DTRANSFORMSTATE_PROJECTION
#define dx_mxw2v            VAR_U_(0x007540D8, D3DMATRIX)
#define dx_mxview           VAR_U_(0x00753B90, D3DMATRIX) // D3DTRANSFORMSTATE_VIEW

#define phd_PutPolygons ((void(__cdecl*)(short* ptr, int clip)) 0x0047DA60)
#define phd_PutPolygons_inv ((void(__cdecl*)(short* ptr, float x, float y, DWORD rgba)) 0x0047E8B0)
#endif