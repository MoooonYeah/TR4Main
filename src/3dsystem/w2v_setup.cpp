#include "framework.h"
#include "w2v_setup.h"
#include "3d_gen.h"

W2V_MATRIX w2v;
void W2V_MATRIX::SetupMatrix_Ptr()
{
    phd_mxptr = matrix_stack;
    mptr = phd_mxptr;
    nptr = &w2v_matrix;
}

void W2V_MATRIX::SetupMatrix_CosAndSin(PHD_3DPOS* viewpos)
{
    this->viewpos = viewpos;
    s.x = phd_sin(viewpos->xRot);
    c.x = phd_cos(viewpos->xRot);
    s.y = phd_sin(viewpos->yRot);
    c.y = phd_cos(viewpos->yRot);
    s.z = phd_sin(viewpos->zRot);
    c.z = phd_cos(viewpos->zRot);
}

void W2V_MATRIX::Matrix00()
{
    auto value = TRIGMULT2(c.y, c.z) + TRIGMULT3(s.x, s.y, s.z);
    mptr->m00 = value;
    nptr->m00 = value;
}

void W2V_MATRIX::Matrix01()
{
    auto value = TRIGMULT2(c.x, s.z);
    mptr->m01 = value;
    nptr->m01 = value;
}

void W2V_MATRIX::Matrix02()
{
    auto value = TRIGMULT3(s.x, c.y, s.z) - TRIGMULT2(s.y, c.z);
    mptr->m02 = value;
    nptr->m02 = value;
}

void W2V_MATRIX::Matrix03()
{
    auto value = viewpos->xPos;
    mptr->m03 = value;
    nptr->m03 = value;
}

void W2V_MATRIX::Matrix10()
{
    auto value = TRIGMULT3(s.x, s.y, c.z) - TRIGMULT2(c.y, s.z);
    mptr->m10 = value;
    nptr->m10 = value;
}

void W2V_MATRIX::Matrix11()
{
    auto value = TRIGMULT2(c.x, c.z);
    mptr->m11 = value;
    nptr->m11 = value;
}

void W2V_MATRIX::Matrix12()
{
    auto value = TRIGMULT2(s.y, s.z) + TRIGMULT3(s.x, c.y, c.z);
    mptr->m12 = value;
    nptr->m12 = value;
}

void W2V_MATRIX::Matrix13()
{
    auto value = viewpos->yPos;
    mptr->m13 = value;
    nptr->m13 = value;
}

void W2V_MATRIX::Matrix20()
{
    auto value = TRIGMULT2(c.x, s.y);
    mptr->m20 = value;
    nptr->m20 = value;
}

void W2V_MATRIX::Matrix21()
{
    auto value = -s.x;
    mptr->m21 = value;
    nptr->m21 = value;
}

void W2V_MATRIX::Matrix22()
{
    auto value = TRIGMULT2(c.x, c.y);
    mptr->m22 = value;
    nptr->m22 = value;
}

void W2V_MATRIX::Matrix23()
{
    auto value = viewpos->zPos;
    mptr->m23 = value;
    nptr->m23 = value;
}

// TODO: check if the poison have the nausea effect ?
void W2V_MATRIX::SetupPoisonEffect()
{
    poison = Lara.poisoned;
    if (Lara.dpoisoned != poison)
    {
        poison = ((Lara.dpoisoned - Lara.poisoned) >> 4) + Lara.poisoned;
        Lara.poisoned = poison;
        if (abs(Lara.dpoisoned - poison) < 16)
        {
            poison = Lara.dpoisoned;
            Lara.poisoned = Lara.dpoisoned;
        }
    }

    if (poison >= 256)
    {
        power = poison - 256;
        angx = power * ((SIN(XSoff1) + SIN(XSoff2)) >> 2);
        angy = power * ((SIN(YSoff1) + SIN(YSoff2)) >> 2);
        angz = power * ((SIN(ZSoff1) + SIN(ZSoff2)) >> 2);
        if (angx || angy || angz)
        {
            pos.x = (angx >> 12) + 0x4000;
            pos.y = (angy >> 12) + 0x4000;
            pos.z = (angz >> 12) + 0x4000;
            ScaleCurrentMatrix(&pos);
        }
    }
}

void W2V_MATRIX::AspectCorrection(int mXX)
{
    int value;

    switch (mXX)
    {
    case M10:
        value = (int)(float(nptr->m10) * LfAspectCorrection);
        nptr->m10 = value;
        mptr->m10 = value;
        break;
    case M11:
        value = (int)(float(nptr->m11) * LfAspectCorrection);
        nptr->m11 = value;
        mptr->m11 = value;
        break;
    case M12:
        value = (int)(float(nptr->m12) * LfAspectCorrection);
        nptr->m12 = value;
        mptr->m12 = value;
        break;
    }
}