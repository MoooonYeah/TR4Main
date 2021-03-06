#include "framework.h"
#include "crocodile.h"
#include "box.h"
#include "control.h"
#include "people.h"
#include "effect2.h"
#include "draw.h"
#include "collide.h"
#include "items.h"

enum CROCODILE_STATE
{
    CROC_EMPTY,
    CROC_IDLE,
    CROC_RUN,
    CROC_WALK,
    CROC_HIT,
    CROC_ATK,
    CROC_EMPTY1,
    CROC_DIE,
    WCROC_SWIM,
    WCROC_ATK,
    WCROC_DIE,
};

#define CROC_WALK_ANGLE ANGLE(3)
#define CROC_SWIM_ANGLE ANGLE(3)
#define CROC_RUN_ANGLE ANGLE(5)

constexpr auto CROC_ANIM_IDLE = 0;
constexpr auto CROC_ANIM_DIE = 11;
constexpr auto CROC_ANIM_SWIM = 12;
constexpr auto CROC_ANIM_WDIE = 16;
constexpr auto CROC_ALERT_RANGE = SQUARE(SECTOR(1) + CLICK(2));
constexpr auto CROC_VISIBILITY_RANGE = SQUARE(SECTOR(5));
constexpr auto CROC_RUN_RANGE = SQUARE(SECTOR(1));
constexpr auto CROC_MAXRUN_RANGE = SQUARE(SECTOR(1) + CLICK(2));
constexpr auto CROC_ATTACK_RANGE = SQUARE(CLICK(2)); // NOTE: TR4 is CLICK(3), but the crocodile not go near lara to do damage in certain case !
constexpr auto CROC_SWIM_SPEED = 16;
constexpr auto CROC_TOUCHBITS = 768;
constexpr auto CROC_DAMAGE = 120;
static BITE_INFO crocBite = { 0, -100, 500, 9 };

// crocodile mode (land or swim) transition anim
constexpr auto CROC_ANIM_SWIM_MODE = 17;
constexpr auto CROC_ANIM_LAND_MODE = 18;

static bool CrocodileIsInWater(ITEM_INFO* item, CREATURE_INFO* crocodile)
{
    EntityStoringInfo info;
    info.x = item->pos.xPos;
    info.y = item->pos.yPos;
    info.z = item->pos.zPos;
    info.roomNumber = item->roomNumber;
    GetFloor(info.x, info.y, info.z, &info.roomNumber);
    info.waterDepth = GetWaterDepth(info.x, info.y, info.z, info.roomNumber);
    if (info.waterDepth != -NO_HEIGHT)
    {
        crocodile->LOT.step = SECTOR(20);
        crocodile->LOT.drop = -SECTOR(20);
        crocodile->LOT.fly = CROC_SWIM_SPEED;
        return true;
    }
    else
    {
        crocodile->LOT.step = CLICK(1);
        crocodile->LOT.drop = -CLICK(1);
        crocodile->LOT.fly = NO_FLYING;
        return false;
    }
}

void InitialiseCrocodile(short itemNumber)
{
    ITEM_INFO* item = &Items[itemNumber];
    InitialiseCreature(itemNumber);

    // if the room is a "water room"
    if (Rooms[item->roomNumber].flags & ROOM_WATER)
    {
        item->animNumber = Objects[item->objectNumber].animIndex + CROC_ANIM_SWIM;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = WCROC_SWIM;
        item->goalAnimState = WCROC_SWIM;
    }
    // then it's a "ground room"
    else
    {
        item->animNumber = Objects[item->objectNumber].animIndex + CROC_ANIM_IDLE;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = CROC_IDLE;
        item->goalAnimState = CROC_IDLE;
    }
}

void CrocodileControl(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item;
    OBJECT_INFO* obj;
    CREATURE_INFO* crocodile;
    AI_INFO info;
    OBJECT_BONES boneRot;
    short angle;
    short bone_angle;

    item = &Items[itemNumber];
    obj = &Objects[item->objectNumber];
    crocodile = GetCreatureInfo(item);
    angle = 0;
    bone_angle = 0;

    if (item->hitPoints <= 0)
    {
        angle = 0;
        bone_angle = 0;

        if (item->currentAnimState != CROC_DIE && item->currentAnimState != WCROC_DIE)
        {
            // water
            if (Rooms[item->roomNumber].flags & ROOM_WATER)
            {
                item->animNumber = obj->animIndex + CROC_ANIM_WDIE;
                item->frameNumber = Anims[item->animNumber].frameBase;
                item->currentAnimState = WCROC_DIE;
                item->goalAnimState = WCROC_DIE;
            }
            // land
            else
            {
                item->animNumber = obj->animIndex + CROC_ANIM_DIE;
                item->frameNumber = Anims[item->animNumber].frameBase;
                item->currentAnimState = CROC_DIE;
                item->goalAnimState = CROC_DIE;
            }
        }

        // creature in water are floating after death.
        if (Rooms[item->roomNumber].flags & ROOM_WATER)
            CreatureFloat(itemNumber);
    }
    else
    {
        if (item->aiBits & ALL_AIOBJ)
            GetAITarget(crocodile);
        else if (crocodile->hurtByLara)
            crocodile->enemy = LaraItem;

        CreatureAIInfo(item, &info);
        GetCreatureMood(item, &info, VIOLENT);
        CreatureMood(item, &info, VIOLENT);
        angle = CreatureTurn(item, crocodile->maximumTurn);

        if ((item->hitStatus || info.distance < CROC_ALERT_RANGE) || (TargetVisible(item, &info) && info.distance < CROC_VISIBILITY_RANGE))
        {
            if (!crocodile->alerted)
                crocodile->alerted = TRUE;
            AlertAllGuards(itemNumber);
        }

        bone_angle = angle << 2;
        switch (item->currentAnimState)
        {
        case CROC_IDLE:
            crocodile->maximumTurn = 0;

            if (item->aiBits & GUARD)
            {
                bone_angle = item->itemFlags[0];
                item->goalAnimState = CROC_IDLE;
                item->itemFlags[0] = item->itemFlags[1] + bone_angle;

                if (!(GetRandomControl() & 0x1F))
                {
                    if (GetRandomControl() & 1)
                        item->itemFlags[1] = 0;
                    else
                        item->itemFlags[1] = (GetRandomControl() & 1) != 0 ? 12 : -12;
                }

                CLAMP(item->itemFlags[0], -1024, 1024);
            }
            else if (info.bite && info.distance < CROC_ATTACK_RANGE)
            {
                item->goalAnimState = CROC_ATK;
            }
            else
            {
                if (info.ahead && info.distance < CROC_RUN_RANGE)
                    item->goalAnimState = CROC_WALK;
                else
                    item->goalAnimState = CROC_RUN;
            }
            break;
        case CROC_WALK:
            crocodile->maximumTurn = CROC_WALK_ANGLE;

            // land to water transition:
            if (CrocodileIsInWater(item, crocodile))
            {
                item->requiredAnimState = WCROC_SWIM;
                item->goalAnimState = WCROC_SWIM;
                break;
            }

            if (item->requiredAnimState)
                item->goalAnimState = item->requiredAnimState;
            else if (info.bite && info.distance < CROC_ATTACK_RANGE)
                item->goalAnimState = CROC_IDLE;
            else if (!info.ahead || info.distance > CROC_MAXRUN_RANGE)
                item->goalAnimState = CROC_RUN;
            break;
        case CROC_RUN:
            crocodile->maximumTurn = CROC_RUN_ANGLE;
            
            // land to water transition:
            if (CrocodileIsInWater(item, crocodile))
            {
                item->requiredAnimState = CROC_WALK;
                item->goalAnimState = CROC_WALK;
                break;
            }

            if (item->requiredAnimState)
                item->goalAnimState = item->requiredAnimState;
            else if (info.bite && info.distance < CROC_ATTACK_RANGE)
                item->goalAnimState = CROC_IDLE;
            else if (info.ahead && info.distance < CROC_RUN_RANGE)
                item->goalAnimState = CROC_WALK;
            break;
        case CROC_ATK:
            if (item->frameNumber == Anims[item->animNumber].frameBase)
                item->requiredAnimState = 0;

            if (info.bite && (item->touchBits & CROC_TOUCHBITS))
            {
                if (!item->requiredAnimState)
                {
                    CreatureEffect2(item, &crocBite, 10, -1, DoBloodSplat);
                    LaraItem->hitPoints -= CROC_DAMAGE;
                    LaraItem->hitStatus = TRUE;
                    item->requiredAnimState = CROC_IDLE;
                }
            }
            else
            {
                item->goalAnimState = CROC_IDLE;
            }
            break;
        case WCROC_SWIM:
            crocodile->maximumTurn = CROC_SWIM_ANGLE;

            // water to land transition:
            if (!CrocodileIsInWater(item, crocodile))
            {
                item->animNumber = obj->animIndex + CROC_ANIM_LAND_MODE;
                item->frameNumber = Anims[item->animNumber].frameBase;
                item->requiredAnimState = CROC_WALK;
                item->currentAnimState = CROC_WALK;
                item->goalAnimState = CROC_WALK;
                break;
            }

            if (item->requiredAnimState)
            {
                item->goalAnimState = item->requiredAnimState;
            }
            else if (info.bite)
            {
                if (item->touchBits & 768)
                    item->goalAnimState = WCROC_ATK;
            }
            break;
        case WCROC_ATK:
            if (item->frameNumber == Anims[item->animNumber].frameBase)
                item->requiredAnimState = CROC_EMPTY;

            if (info.bite && (item->touchBits & CROC_TOUCHBITS))
            {
                if (!item->requiredAnimState)
                {
                    CreatureEffect2(item, &crocBite, 10, -1, DoBloodSplat);
                    LaraItem->hitPoints -= CROC_DAMAGE;
                    LaraItem->hitStatus = TRUE;
                    item->requiredAnimState = WCROC_SWIM;
                }
            }
            else
            {
                item->goalAnimState = WCROC_SWIM;
            }
            break;
        }
    }

    if (item->currentAnimState == CROC_IDLE || item->currentAnimState == CROC_ATK || item->currentAnimState == WCROC_ATK)
        boneRot = OBJECT_BONES(info.angle, info.xAngle);
    else
        boneRot = OBJECT_BONES(bone_angle);

    CreatureTilt(item, 0);
    CreatureJoint(item, 0, boneRot.bone0);
    CreatureJoint(item, 1, boneRot.bone1);
    CreatureJoint(item, 2, boneRot.bone2);
    CreatureJoint(item, 3, boneRot.bone3);

    if (item->currentAnimState < WCROC_SWIM)
        CalcItemToFloorRotation(item, 2);
    
    CreatureAnimation(itemNumber, angle, 0);

    if (item->currentAnimState >= WCROC_SWIM && item->currentAnimState <= WCROC_DIE)
        CreatureUnderwater(item, CLICK(1));
    else
        CreatureUnderwater(item, CLICK(0));
}

#ifdef DLL_INJECT
void injector::f_game::inject_crocodile()
{
    inject(0x00402D90, InitialiseCrocodile);
    inject(0x00402E30, CrocodileControl);
}
#endif
