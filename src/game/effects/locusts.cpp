#include "framework.h"
#include "locusts.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "control.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "collide.h"
#include "draw.h"
#include "effect2.h"

LOCUST_INFO Locusts[MAX_LOCUSTS];
constexpr auto LOCUST_LARA_DAMAGE = 3;
constexpr auto LOCUST_ENTITY_DAMAGE = 1;

int CreateLocust(void)
{
    LOCUST_INFO* locust;

    for (int i = 0; i < MAX_LOCUSTS; i++)
    {
        locust = &Locusts[i];
        if (!locust->on)
            return i;
    }

    return NO_ITEM;
}

void SpawnLocust(ITEM_INFO* item)
{
    LOCUST_INFO* locust;
    ITEM_INFO* target;
    PHD_VECTOR start, end;
    short angles[2];
    short locustNumber = NO_ITEM;

    locustNumber = CreateLocust();
    if (locustNumber != NO_ITEM)
    {
        locust = &Locusts[locustNumber];
        // emitter
        if (item->objectNumber == LOCUSTS_EMITTER)
        {
            end.x = item->pos.xPos;
            end.y = item->pos.yPos;
            end.z = item->pos.zPos;
            angles[0] = item->pos.yRot - ANGLE(180);
            angles[1] = 0;
        }
        // mutant
        else
        {
            start.x = 0;
            start.y = -96;
            start.z = 144;
            GetJointAbsPosition(item, &start, 9);
            end.x = 0;
            end.y = -128;
            end.z = 288;
            GetJointAbsPosition(item, &end, 9);
            phd_GetVectorAngles(end.x - start.x, end.y - start.y, end.z - start.z, angles);
        }

        target = GetCreatureInfo(item)->enemy;

        locust->on = true;
        locust->target = target != nullptr ? target : nullptr;
        locust->pos.xPos = end.x;
        locust->pos.yPos = end.y;
        locust->pos.zPos = end.z;
        locust->pos.yRot = (GetRandomControl() & 0x7FF) + angles[0] - 0x400;
        locust->pos.xRot = (GetRandomControl() & 0x3FF) + angles[1] - 0x200;
        locust->roomNumber = item->roomNumber;
        locust->randomRotation = (GetRandomControl() & 0x1F) + 0x10;
        locust->escapeYrot = (GetRandomControl() & 0x1FF);
        locust->escapeXrot = ((GetRandomControl() & 0x7) + 0xF) * 20;
        locust->counter = 0;
    }
}

void InitialiseLocust(short itemNumber)
{
    ITEM_INFO* item = &Items[itemNumber];

    if (item->pos.yRot > 0)
    {
        if (item->pos.yRot == 0x4000)
            item->pos.xPos += CLICK(2);
    }
    else if (item->pos.yRot < 0)
    {
        if (item->pos.yRot == -0x8000)
            item->pos.zPos -= CLICK(2);
        else if (item->pos.yRot == -0x4000)
            item->pos.xPos -= CLICK(2);
    }
    else
    {
        item->pos.zPos += CLICK(2);
    }
}

void LocustControl(short itemNumber)
{
    ITEM_INFO* item = &Items[itemNumber];

    if (TriggerActive(item))
    {
        if (item->triggerFlags)
        {
            SpawnLocust(item);
            item->triggerFlags--;
        }
        else
        {
            KillItem(itemNumber);
        }
    }
}

void UpdateLocusts(void)
{
    LOCUST_INFO* locust;
    int distance, bestdistance;
    int square;
    short angles[2];

    bestdistance = MAXINT;
    for (int i = 0; i < MAX_LOCUSTS; i++)
    {
        locust = &Locusts[i];
        if (locust->on)
        {
            if (locust->target == nullptr)
                locust->target = LaraItem;

            if (locust->target->hitPoints <= 0 && locust->counter >= 90 && !(GetRandomControl() & 7))
                locust->counter = 90;

            locust->counter--;
            if (locust->counter == 0)
            {
                locust->on = FALSE;
                break;
            }

            if (!(GetRandomControl() & 7))
            {
                locust->escapeYrot = GetRandomControl() % 640 + 128;
                locust->escapeXrot = (GetRandomControl() & 0x7F) - 64;
                locust->escapeZrot = (GetRandomControl() & 0x7F) - 64;
            }

            phd_GetVectorAngles(
                locust->target->pos.xPos + 8 * locust->escapeXrot - locust->pos.xPos,
                locust->target->pos.yPos - locust->escapeYrot - locust->pos.yPos,
                locust->target->pos.zPos + 8 * locust->escapeZrot - locust->pos.zPos,
                angles);

            distance = SQUARE(locust->target->pos.zPos - locust->pos.zPos) + SQUARE(locust->target->pos.xPos - locust->pos.xPos);
            square = phd_sqrt(distance) >> 3;
            if (square <= 128)
            {
                if (square < 48)
                    square = 48;
            }
            else
            {
                square = 128;
            }

            if (locust->randomRotation < square)
                locust->randomRotation += 1;
            else if (locust->randomRotation > square)
                locust->randomRotation -= 1;

            if (locust->counter > 90)
            {
                short resultYrot, resultXrot;
                int shiftYrot, shiftXrot;
                int random = locust->randomRotation << 7;
                resultYrot = angles[0] - locust->pos.yRot;
                if (abs(resultYrot) > 0x8000)
                    resultYrot = locust->pos.yRot - angles[0];
                resultXrot = angles[1] - locust->pos.xRot;
                if (abs(resultXrot) > 0x8000)
                    resultXrot = locust->pos.xRot - angles[0];
                shiftYrot = resultYrot >> 3;
                shiftXrot = resultXrot >> 3;
                if (shiftYrot > random || shiftYrot < -random)
                    shiftYrot = -random;
                if (shiftXrot > random || shiftXrot < -random)
                    shiftXrot = -random;
                locust->pos.yRot += shiftYrot;
                locust->pos.xRot += shiftXrot;
            }

            locust->pos.xPos += (locust->randomRotation * COS(locust->pos.xRot) >> W2V_SHIFT) * SIN(locust->pos.yRot) >> W2V_SHIFT;
            locust->pos.yPos +=  locust->randomRotation * SIN(-locust->pos.xRot) >> W2V_SHIFT;
            locust->pos.zPos += (locust->randomRotation * COS(locust->pos.xRot) >> W2V_SHIFT) * COS(locust->pos.yRot) >> W2V_SHIFT;

            if (ItemNearTarget(&locust->pos, locust->target, CLICK(1) / 2))
            {
                TriggerBlood(locust->pos.xPos, locust->pos.yPos, locust->pos.zPos, 2 * GetRandomControl(), 2);
                if (locust->target == LaraItem)
                    locust->target->hitPoints -= LOCUST_LARA_DAMAGE;
                else
                    locust->target->hitPoints -= LOCUST_ENTITY_DAMAGE;
            }

            if (locust->counter > 0)
                SoundEffect(SFX_LOCUSTS_LOOP, &locust->pos, NULL);
        }
    }
}

void DrawLocust(void)
{
    LOCUST_INFO* locust;
    for (int i = 0; i < MAX_LOCUSTS; i++)
    {
        locust = &Locusts[i];
        if (locust->on)
        {
            phd_PushMatrix();
            phd_TranslateAbs(locust->pos.xPos, locust->pos.yPos, locust->pos.zPos);
            phd_RotYXZ(locust->pos.yRot, locust->pos.xRot, locust->pos.zRot);
            phd_PutPolygonsLocust(Meshes[Objects[AHMET_MIP].meshIndex + ((-GlobalCounter & 3) * 2)], 0);
            phd_PopMatrix();
        }
    }
}

#ifdef DLL_INJECT
void injector::f_game::inject_locusts()
{
    inject(0x004035D0, CreateLocust);
    inject(0x00403640, SpawnLocust);
    inject(0x004037B0, InitialiseLocust);
    inject(0x00403810, LocustControl);
    inject(0x00403870, UpdateLocusts);
    inject(0x00403C10, DrawLocust);
}
#endif
