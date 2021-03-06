#include "framework.h"
#include "mutant.h"
#include "3d_gen.h"
#include "box.h"
#include "effect2.h"
#include "control.h"
#include "people.h"
#include "sphere.h"
#include "locusts.h"
#include "items.h"

enum MUTANT_STATE
{
    MUTANT_EMPTY,
    MUTANT_APPEAR,
    MUTANT_IDLE,
    MUTANT_SHOOT,
    MUTANT_LOCUST1,
    MUTANT_LOCUST2,
};

enum class MissileRotationType
{
    M_FRONT,
    M_LEFT,
    M_RIGHT
};

constexpr auto MUTANT_ANIM_APPEAR = 0;
constexpr auto MUTANT_SHOOT_RANGE = SQUARE(SECTOR(10));
constexpr auto MUTANT_LOCUST1_RANGE = SQUARE(SECTOR(15));
constexpr auto MUTANT_LOCUST2_RANGE = SQUARE(SECTOR(30));

static void TriggerMutantRocket(PHD_3DPOS* src, short roomNumber, short counter)
{
    FX_INFO* fx;
    short fxNumber = NO_ITEM;

    fxNumber = CreateEffect(roomNumber);
    if (fxNumber != NO_ITEM)
    {
        fx = &Effects[fxNumber];
        fx->pos.xPos = src->xPos;
        fx->pos.yPos = src->yPos - (GetRandomControl() & 0x3F) - 32;
        fx->pos.zPos = src->zPos;
        fx->pos.xRot = src->xRot;
        fx->pos.yRot = src->yRot;
        fx->pos.zRot = 0;
        fx->roomNumber = roomNumber;
        fx->counter = 16 * counter + 15;
        fx->objectNumber = BUBBLES;
        fx->frameNumber = Objects[fx->objectNumber].meshIndex + 5 * 2;
        fx->speed = (GetRandomControl() & 0x1F) + 96;
        fx->flag1 = 6;
    }
}

void TriggerMutantRocketEffects(short fxNumber, short xVel, short yVel, short zVel)
{
    FX_INFO* fx;
    SPARKS* sptr;
    BYTE color, life, size;

    //x = LaraItem->pos.xPos - Effects[m_fxNumber].pos.xPos;
    //z = LaraItem->pos.zPos - Effects[m_fxNumber].pos.zPos;
    //if (x >= -0x4000u && x <= 0x4000 && z >= -0x4000u && z <= 0x4000)

    fx = &Effects[fxNumber];
    sptr = &Sparks[GetFreeSparks()];
    sptr->on = TRUE;
    color = (GetRandomControl() & 0x3F) - 128;
    sptr->sB = 0;
    sptr->sR = color;
    sptr->sG = color >> 1;
    color = (GetRandomControl() & 0x3F) - 128;
    sptr->dB = 0;
    sptr->dR = color;
    sptr->dG = color >> 1;
    sptr->fadeToBlack = 8;
    sptr->colFadeSpeed = (GetRandomControl() & 3) + 8;
    sptr->transtype = TS_COLADD;
    sptr->dynamic = -1;
    life = (GetRandomControl() & 7) + 32;
    sptr->life = life;
    sptr->sLife = life;
    sptr->x = (GetRandomControl() & 0xF) - 8;
    sptr->y = 0;
    sptr->z = (GetRandomControl() & 0xF) - 8;
    sptr->x += fx->pos.xPos;
    sptr->y += fx->pos.yPos;
    sptr->z += fx->pos.zPos;
    sptr->xVel = xVel;
    sptr->yVel = yVel;
    sptr->zVel = zVel;
    sptr->friction = 34;
    sptr->flags = SPF_EXPDEF | SPF_ROTATE | SPF_DEF | SPF_SCALE;
    sptr->rotAng = GetRandomControl() & 0xFFF;
    if (GetRandomControl() & 1)
        sptr->rotAdd = (GetRandomControl() & 0x1F) - 32;
    else
        sptr->rotAdd = (GetRandomControl() & 0x1F) + 32;
    sptr->gravity = 0;
    sptr->maxyvel = 0;
    sptr->fxObj = BYTE(fxNumber);
    sptr->scalar = 2;
    size = (GetRandomControl() & 0xF) + 128;
    sptr->size = size;
    sptr->sSize = size;
    sptr->dSize = size >> 2;
}

static void ShootFireball(PHD_3DPOS* src, MissileRotationType rotation, short roomNumber, int timer)
{
    switch (rotation)
    {
        case MissileRotationType::M_LEFT:
            src->yRot -= GetRandomControl() % 0x2000;
            break;
        case MissileRotationType::M_RIGHT:
            src->yRot += GetRandomControl() % 0x2000;
            break;
    }

    TriggerMutantRocket(src, roomNumber, timer);
}

static bool ShootFrame(ITEM_INFO* item)
{
    short frameNumber = GetCurrentFrame(item);
    if (frameNumber == 45
    //||  frameNumber == 50
    //||  frameNumber == 55
    ||  frameNumber == 60
    //||  frameNumber == 65
    //||  frameNumber == 70
    ||  frameNumber == 75)
        return true;
    else
        return false;
}

static void RotateHeadToTarget(ITEM_INFO* item, CREATURE_INFO* creature, int joint, short& headAngle)
{
    if (creature->enemy == nullptr)
    {
        headAngle = item->pos.yRot;
        return;
    }
    ITEM_INFO* enemy = creature->enemy;
    PHD_VECTOR pos;
    int x, z;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    GetJointAbsPosition(item, &pos, joint);
    x = enemy->pos.xPos - pos.x;
    z = enemy->pos.zPos - pos.z;
    headAngle = (short)(phd_atan(z, x) - item->pos.yRot) / 2;
}

static void GetTargetPosition(ITEM_INFO* item, PHD_3DPOS* target)
{
    PHD_VECTOR start, end;
    short angles[2];
    start.x = 0;
    start.y = -96;
    start.z = 144;
    GetJointAbsPosition(item, &start, 9);
    end.x = 0;
    end.y = -128;
    end.z = 288;
    GetJointAbsPosition(item, &end, 9);
    phd_GetVectorAngles(end.x - start.x, end.y - start.y, end.z - start.z, angles);
    target->xPos = end.x;
    target->yPos = end.y;
    target->zPos = end.z;
    target->yRot = angles[0];
    target->xRot = angles[1];
    target->zRot = 0;
}

static void MoveItemFront(ITEM_INFO* item, int distance)
{
    switch (short(to_degrees(item->pos.yRot)))
    {
    case C_NORTH:
        item->pos.zPos += distance;
        break;
    case C_EAST:
        item->pos.xPos += distance;
        break;
    case C_SOUTH:
        item->pos.zPos -= distance;
        break;
    case C_WEST:
        item->pos.xPos -= distance;
        break;
    }
}

static void MoveItemBack(ITEM_INFO* item, int distance)
{
    switch (short(to_degrees(item->pos.yRot)))
    {
    case C_NORTH:
        item->pos.zPos -= distance;
        break;
    case C_EAST:
        item->pos.xPos -= distance;
        break;
    case C_SOUTH:
        item->pos.zPos += distance;
        break;
    case C_WEST:
        item->pos.xPos += distance;
        break;
    }
}

static void MutantAIFix(ITEM_INFO* item, AI_INFO* info)
{
    MoveItemFront(item, SECTOR(2));
    item->pos.yPos -= CLICK(3);
    CreatureAIInfo(item, info);
    item->pos.yPos += CLICK(3);
    MoveItemBack(item, SECTOR(2));
}

void InitialiseMutant(short itemNumber)
{
    ITEM_INFO* item;
    InitialiseCreature(itemNumber);

    item = &Items[itemNumber];
    item->animNumber = Objects[item->objectNumber].animIndex + MUTANT_ANIM_APPEAR;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = MUTANT_APPEAR;
    item->goalAnimState = MUTANT_APPEAR;
}

void MutantControl(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item;
    CREATURE_INFO* mutant;
    AI_INFO info;
    OBJECT_BONES mutant_joint;
    short frameNumber;
    short headY;
    short angle;
    
    item = &Items[itemNumber];
    mutant = GetCreatureInfo(item);
    angle = 0;
    headY = 0;

    if (item->aiBits & ALL_AIOBJ)
        GetAITarget(mutant);
    else if (mutant->hurtByLara)
        mutant->enemy = LaraItem;
    else
        TargetNearestEntity(item, mutant);

    MutantAIFix(item, &info);
    RotateHeadToTarget(item, mutant, 9, headY);
    GetCreatureMood(item, &info, VIOLENT);
    CreatureMood(item, &info, VIOLENT);
    mutant->maximumTurn = 0;
    angle = CreatureTurn(item, 0);

    switch (item->currentAnimState)
    {
    case MUTANT_IDLE:
        if (info.ahead)
        {
            int random = GetRandomControl() & 31;
            if ((random > 0 && random < 10) && info.distance <= MUTANT_SHOOT_RANGE)
                item->goalAnimState = MUTANT_SHOOT;
            else if ((random > 10 && random < 20) && info.distance <= MUTANT_LOCUST1_RANGE)
                item->goalAnimState = MUTANT_LOCUST1;
            else if ((random > 20 && random < 30) && info.distance <= MUTANT_LOCUST2_RANGE)
                item->goalAnimState = MUTANT_LOCUST2;
        }
        break;
    case MUTANT_SHOOT:
        frameNumber = GetCurrentFrame(item);
        if (frameNumber >= 94 && frameNumber <= 96)
        {
            PHD_3DPOS src;
            GetTargetPosition(item, &src);
            if (frameNumber == 94)
            {
                ShootFireball(&src, MissileRotationType::M_FRONT, item->roomNumber, 0);
            }
            else if (frameNumber == 95)
            {
                ShootFireball(&src, MissileRotationType::M_LEFT, item->roomNumber, 1);
                //ShootFireball(&src, MissileRotationType::M_LEFT, item->roomNumber, 1);
            }
            else if (frameNumber == 96)
            {
                ShootFireball(&src, MissileRotationType::M_RIGHT, item->roomNumber, 1);
                //ShootFireball(&src, MissileRotationType::M_RIGHT, item->roomNumber, 1);
            }
        }
        break;
    case MUTANT_LOCUST1:
        frameNumber = GetCurrentFrame(item);
        if (frameNumber >= 60 && frameNumber <= 120)
            SpawnLocust(item);
        break;
    case MUTANT_LOCUST2:
        if (ShootFrame(item))
        {
            PHD_3DPOS src;
            GetTargetPosition(item, &src);
            ShootFireball(&src, MissileRotationType::M_FRONT, item->roomNumber, 1);
        }
        break;
    }

    if (item->currentAnimState != MUTANT_LOCUST1)
        mutant_joint = OBJECT_BONES(headY, info.xAngle, true);
    else
        mutant_joint = OBJECT_BONES(0);

    CreatureJoint(item, 0, mutant_joint.bone0);
    CreatureJoint(item, 1, mutant_joint.bone1);
    CreatureJoint(item, 2, mutant_joint.bone2);
    CreatureJoint(item, 3, mutant_joint.bone3);
    CreatureAnimation(itemNumber, angle, 0);
}

#ifdef DLL_INJECT
void injector::f_game::inject_mutant()
{
    inject(0x00403ED0, InitialiseMutant);
    inject(0x00403F30, MutantControl);
    inject(0x00403C90, TriggerMutantRocket);
    inject(0x00403D30, TriggerMutantRocketEffects);
}
#endif
