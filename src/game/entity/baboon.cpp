#include "framework.h"
#include "baboon.h"
#include "box.h"
#include "effect2.h"
#include "lot.h"
#include "items.h"
#include "draw.h"
#include "control.h"
#include "3d_gen.h"

BABOON_RESPAWN BaboonRespawn;
static BITE_INFO baboonBite = { 10, 10, 11, 4 };

enum BABOON_STATE
{
    BABOON_NULL,
    BABOON_EMPTY,
    BABOON_WALK,
    BABOON_IDLE,
    BABOON_RUN,
    BABOON_PICKUP,
    BABOON_SIT_IDLE,
    BABOON_SIT_EAT,
    BABOON_SIT_SCRATCH,
    BABOON_RUN_ROLL,
    BABOON_HITGROUND,
    BABOON_DEATH,
    BABOON_ATK1,
    BABOON_JUMPATK,
    BABOON_SUPERJUMPATK,
    /// NOT USED IN TR4:
    BABOON_CLIMB_4CLICK,
    BABOON_CLIMB_3CLICK,
    BABOON_CLIMB_2CLICK,
    BABOON_FALL_4CLICK,
    BABOON_FALL_3CLICK,
    BABOON_FALL_2CLICK,
    ///!END
    BABOON_ACTIVATE_SWITCH
};

constexpr auto NO_BABOON = -1;
constexpr auto NO_BABOON_COUNT = -2;
constexpr auto NO_CROWBAR_SWITCH_FOUND = -1;

#define BABOON_SIT_IDLE_ANIM 2
#define BABOON_IDLE_ANIM 9
#define BABOON_DEATH_ANIM 14
#define BABOON_SWITCH_ANIM 31

#define BABOON_DAMAGE 70
#define BABOON_IDLE_DISTANCE SQUARE(WALL_L)
#define BABOON_ATTACK_ANGLE ANGLE(7)
#define BABOON_ATK_RANGE 0x718E4
#define BABOON_ATK_NORMALRANGE 0x1C639
#define BABOON_JUMP_RANGE 0x718E4
#define BABOON_FOLLOW_RANGE 0x400000
#define BABOON_RUNROLL_RANGE 0x100000
#define BABOON_WALK_ANGLE ANGLE(7)
#define BABOON_RUN_ANGLE ANGLE(11)
#define BABOON_RIGHT_TOUCHBITS 814
#define BABOON_JUMP_TOUCHBITS 280
#define BABOON_TOUCHBITS 0x1800

static void KillRespawnedBaboon(short itemNumber, bool remove = false)
{
    ITEM_INFO* item;
    item = &Items[itemNumber];
    item->hitPoints = 0;
    RemoveActiveItem(itemNumber); // remove it from the active item list
    item->flags = IFLAG_CLEAR_BODY;
    item->afterDeath = 128; // instant disappear !
    item->status = FITEM_DEACTIVATED; // wont triggered again...
    if (remove)
        item->itemFlags[0] = NO_BABOON;
    DisableBaddieAI(itemNumber); // desactivate this AI or you will get crash later...
}

static bool CheckRespawnedBaboon(short itemNumber)
{
    ITEM_INFO* item;
    BABOON_RESPAWN_STRUCT* baboon;

    item = &Items[itemNumber];
    if (item->itemFlags[0] == NO_BABOON) // NORMAL/INV for now
    {
        KillRespawnedBaboon(itemNumber);
        return false;
    }
    baboon = BaboonRespawn.GetBaboonRespawn(item->itemFlags[0]);
    if (baboon == nullptr)
        return false;
    if (baboon->count == baboon->max_count)
    {
        KillRespawnedBaboon(itemNumber, true);
        return false;
    }
    return true;
}

static void UpdateRespawnedBaboon(short itemNumber)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;
    BABOON_RESPAWN_STRUCT* baboon;

    item = &Items[itemNumber];
    obj = &Objects[item->objectNumber];
    baboon = BaboonRespawn.GetBaboonRespawn(item->itemFlags[0]);
    if (baboon == nullptr)
        return;

    item->pos = baboon->pos;
    IsRoomOutside(item->pos.xPos, item->pos.yPos, item->pos.zPos);
    if (item->roomNumber != IsRoomOutsideNo)
        ItemNewRoom(itemNumber, IsRoomOutsideNo);
    if (baboon->count < baboon->max_count)
        baboon->count++;

    item->animNumber = obj->animIndex + BABOON_SIT_IDLE_ANIM;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = BABOON_SIT_IDLE;
    item->goalAnimState = BABOON_SIT_IDLE;
    item->hitPoints = obj->hitPoints;

    RemoveActiveItem(itemNumber);
    item->flags = IFLAG_NONE;
    item->afterDeath = 0;
    item->status = FITEM_INVISIBLE;

    DisableBaddieAI(itemNumber);
    if (item->objectNumber == BABOON_NORMAL)
    {
        if (item->triggerFlags == 1)
            return;
        else
            item->collidable = TRUE;
    }
    else if (item->triggerFlags == 0)
    {
        item->collidable = TRUE;
    }

    BaboonSearchCrowbarSwitch(item, 0);
}

void InitialiseBaboon(short itemNumber)
{
    ITEM_INFO* item;
    InitialiseCreature(itemNumber);

    item = &Items[itemNumber];
    item->animNumber = Objects[item->objectNumber].animIndex + BABOON_SIT_IDLE_ANIM;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->goalAnimState = BABOON_SIT_IDLE;
    item->currentAnimState = BABOON_SIT_IDLE;

    if (item->objectNumber == BABOON_SILENT)
        BaboonRespawn.Add(item, 2);
    else
        item->itemFlags[0] = NO_BABOON;
}

void BaboonControl(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item;
    CREATURE_INFO* baboon;
    FLOOR_INFO* floor;
    AI_INFO info, lara_info;
    short tilt, angle, head_y;

    item = &Items[itemNumber];
    baboon = GetCreatureInfo(item);
    head_y = 0;
    tilt = 0;
    angle = 0;
    
    if (item->hitPoints <= 0 && item->hitPoints != DONT_TARGET)
    {
        if (item->currentAnimState == BABOON_DEATH)
        {
            if (item->frameNumber == Anims[item->animNumber].frameEnd)
                BaboonRespawnFunction(itemNumber);
        }
        else if (item->currentAnimState != BABOON_ACTIVATE_SWITCH)
        {
            item->animNumber = Objects[item->objectNumber].animIndex + BABOON_DEATH_ANIM;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->currentAnimState = BABOON_DEATH;
            item->goalAnimState = BABOON_DEATH;
        }
    }
    else
    {
        GetAITarget(baboon);
        CreatureAIInfo(item, &info);

        if (!item->hitStatus && item->objectNumber == BABOON_NORMAL)
        {
            int dx, dz;
            dx = LaraItem->pos.xPos - item->pos.xPos;
            dz = LaraItem->pos.zPos - item->pos.zPos;
            lara_info.angle = phd_atan(dx, dz) - item->pos.yRot;
            lara_info.distance = SQUARE(dx) + SQUARE(dz);
            if (baboon->enemy == nullptr || baboon->enemy == LaraItem)
                baboon->enemy = nullptr;
        }
        else
        {
            lara_info.angle = info.angle;
            lara_info.distance = info.distance;
            baboon->enemy = LaraItem;
        }

        GetCreatureMood(item, &info, VIOLENT);
        CreatureMood(item, &info, VIOLENT);
        angle = CreatureTurn(item, baboon->maximumTurn);

        if (baboon->enemy != nullptr && baboon->enemy != LaraItem && baboon->enemy->objectNumber == AI_FOLLOW)
        {
            if (baboon->reachedGoal
            && (abs(item->pos.xPos - baboon->enemy->pos.xPos) < STEP_L
            &&  abs(item->pos.yPos - baboon->enemy->pos.yPos) < STEP_L
            &&  abs(item->pos.zPos - baboon->enemy->pos.zPos) < STEP_L))
            {
                item->pos.xPos = baboon->enemy->pos.xPos;
                item->pos.yPos = baboon->enemy->pos.yPos;
                item->pos.zPos = baboon->enemy->pos.zPos;
                item->pos.yRot = baboon->enemy->pos.yRot;
                item->animNumber = Objects[item->objectNumber].animIndex + BABOON_SWITCH_ANIM;
                item->frameNumber = Anims[item->animNumber].frameBase;
                item->goalAnimState = BABOON_ACTIVATE_SWITCH;
                item->currentAnimState = BABOON_ACTIVATE_SWITCH;
                item->aiBits &= ~(FOLLOW);
                floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &item->roomNumber);
                GetFloorHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
                TestTriggers(TriggerIndex, TRUE, FALSE);
                baboon->enemy = nullptr;
            }
        }

        switch (item->currentAnimState)
        {
        case BABOON_IDLE:
            baboon->maximumTurn = 0;
            baboon->flags = 0;

            if (item->aiBits & GUARD)
            {
                AIGuard(baboon);
                if (!(GetRandomControl() & 0xF))
                {
                    if (GetRandomControl() & 1)
                        item->goalAnimState = BABOON_HITGROUND;
                    else
                        item->goalAnimState = BABOON_SIT_IDLE;
                }
            }
            else if (item->aiBits & PATROL1)
            {
                item->goalAnimState = BABOON_WALK;
            }
            else if (baboon->mood == ESCAPE_MOOD)
            {
                if (info.ahead && Lara.target != item)
                    item->goalAnimState = BABOON_IDLE;
                else
                    item->goalAnimState = BABOON_RUN;
            }
            else if (baboon->mood == ATTACK_MOOD)
            {
                if (!(item->aiBits & FOLLOW) || (!item->gravityStatus && info.distance <= BABOON_RUNROLL_RANGE))
                {
                    if (info.bite && info.distance < BABOON_ATK_NORMALRANGE)
                    {
                        if (LaraItem->pos.yPos >= item->pos.yPos)
                            item->goalAnimState = BABOON_ATK1;
                        else
                            item->goalAnimState = BABOON_JUMPATK;
                    }
                    else if (info.bite && info.distance < BABOON_JUMP_RANGE)
                    {
                        item->goalAnimState = BABOON_SUPERJUMPATK;
                    }
                    else if (info.bite && info.distance < BABOON_RUNROLL_RANGE)
                    {
                        item->goalAnimState = BABOON_RUN_ROLL;
                    }
                    else
                    {
                        item->goalAnimState = BABOON_RUN;
                    }
                }
                else if (item->requiredAnimState)
                {
                    item->goalAnimState = item->requiredAnimState;
                }
                else if (GetRandomControl() & 1)
                {
                    item->goalAnimState = BABOON_SIT_IDLE;
                }
            }
            else if (item->requiredAnimState)
            {
                item->goalAnimState = item->requiredAnimState;
            }
            else if (!(GetRandomControl() & 3))
            {
                item->goalAnimState = BABOON_WALK;
            }
            else if (!(GetRandomControl() & 1))
            {
                item->goalAnimState = BABOON_RUN_ROLL;
            }
            else if (GetRandomControl() & 4)
            {
                item->goalAnimState = BABOON_HITGROUND;
            }
            break;
        case BABOON_SIT_IDLE:
            baboon->maximumTurn = 0;
            baboon->flags = 0;

            if (item->aiBits & GUARD)
            {
                AIGuard(baboon);
                if (GetRandomControl() & 0xF)
                    item->goalAnimState = BABOON_SIT_EAT;
                else if (GetRandomControl() & 0xA)
                    item->goalAnimState = BABOON_SIT_SCRATCH;
            }
            else if (item->aiBits & PATROL1)
            {
                item->goalAnimState = BABOON_WALK;
            }
            else if (baboon->mood != ESCAPE_MOOD)
            {
                if (baboon->mood == BORED_MOOD)
                {
                    if (item->requiredAnimState)
                    {
                        item->goalAnimState = item->requiredAnimState;
                    }
                    // NOTE: it's not the original code, but it's too wreid 
                    // that the baboon repeat the same move so i included the sit_idle with more random number
                    // (the eat not exist in the bored mood, i added it !)
                    else if (GetRandomControl() & 0x10)
                    {
                        item->goalAnimState = BABOON_SIT_IDLE;
                    }
                    else if (GetRandomControl() & 0x500)
                    {
                        if (GetRandomControl() & 0x200)
                            item->goalAnimState = BABOON_SIT_SCRATCH;
                        else if (GetRandomControl() & 0x250)
                            item->goalAnimState = BABOON_SIT_EAT;
                    }
                    else if (GetRandomControl() & 0x1000 || item->aiBits & FOLLOW)
                    {
                        item->goalAnimState = BABOON_WALK;
                    }
                }
                else if ((item->aiBits & FOLLOW) && info.distance > BABOON_IDLE_DISTANCE)
                {
                    if (item->requiredAnimState)
                        item->goalAnimState = item->requiredAnimState;
                    else
                        item->goalAnimState = BABOON_WALK;
                }
                else
                {
                    item->goalAnimState = BABOON_WALK;
                }
            }
            else
            {
                item->goalAnimState = BABOON_IDLE;
            }
            break;
        case BABOON_WALK:
            baboon->maximumTurn = BABOON_WALK_ANGLE;

            if (item->aiBits & PATROL1)
            {
                item->goalAnimState = BABOON_WALK;
            }
            else if (baboon->mood == BORED_MOOD)
            {
                if (item->aiBits & FOLLOW)
                    item->goalAnimState = BABOON_WALK;
                else if (GetRandomControl() < 256)
                    item->goalAnimState = BABOON_SIT_IDLE;
            }
            else if (baboon->mood == ESCAPE_MOOD)
            {
                item->goalAnimState = BABOON_RUN;
            }
            else if (baboon->mood == ATTACK_MOOD)
            {
                if (info.bite && info.distance < BABOON_ATK_RANGE)
                    item->goalAnimState = BABOON_IDLE;
            }
            else if (GetRandomControl() < 256)
            {
                item->goalAnimState = BABOON_SIT_IDLE;
            }
            break;
        case BABOON_RUN:
            baboon->maximumTurn = BABOON_RUN_ANGLE;
            tilt = angle / 2;

            if (item->aiBits & GUARD)
            {
                item->goalAnimState = BABOON_IDLE;
            }
            else if (baboon->mood == ESCAPE_MOOD)
            {
                if (info.ahead && Lara.target != item)
                    item->goalAnimState = BABOON_IDLE;
            }
            else if (item->aiBits & FOLLOW && (item->gravityStatus || info.distance > BABOON_FOLLOW_RANGE))
            {
                item->goalAnimState = BABOON_IDLE;
            }
            else if (baboon->mood == ATTACK_MOOD)
            {
                if (info.distance < BABOON_ATK_RANGE)
                    item->goalAnimState = BABOON_IDLE;
                else if (info.bite && info.distance < BABOON_RUNROLL_RANGE)
                    item->goalAnimState = BABOON_RUN_ROLL;
            }
            else
            {
                item->goalAnimState = BABOON_RUN_ROLL;
            }
            break;
        case BABOON_PICKUP:
            baboon->maximumTurn = 0;
            // NOTE: baboon not use it ! (only TR3 one)
            break;
        case BABOON_ACTIVATE_SWITCH:
            baboon->maximumTurn = 0;
            item->hitPoints = DONT_TARGET;
            
            if (item->frameNumber == Anims[item->animNumber].frameBase + 212)
            {
                GAME_VECTOR pos;
                pos.x = 0;
                pos.y = 0;
                pos.z = 0;
                pos.boxNumber = 0;
                pos.roomNumber = NO_ROOM;

                switch (item->pos.yRot)
                {
                case -0x4000: // WEST (OK)
                    pos.x = item->pos.xPos - SECTOR(1);
                    pos.z = item->pos.zPos;
                    break;
                case 0x4000: // EAST (OK)
                    pos.x = item->pos.xPos + SECTOR(1);
                    pos.z = item->pos.zPos;
                    break;
                case 0:      // NORTH (NOP)
                    pos.x = item->pos.xPos;
                    pos.z = item->pos.zPos + SECTOR(1);
                    break;
                case -0x8000: // SOUTH (OK)
                    pos.x = item->pos.xPos;
                    pos.z = item->pos.zPos - SECTOR(1);
                    break;
                }

                pos.y = item->pos.yPos;
                pos.roomNumber = item->roomNumber;
                floor = GetFloor(pos.x, pos.y, pos.z, &pos.roomNumber);
                int height = GetFloorHeight(floor, pos.x, pos.y, pos.z);
                item->floor = height;
                TestTriggerXYZ(pos.x, pos.y, pos.z, pos.roomNumber, TRUE, NULL);
                item->triggerFlags = 1;
            }
            break;
        case BABOON_ATK1:
        case BABOON_JUMPATK:
        case BABOON_SUPERJUMPATK:
            if (info.ahead)
                head_y = info.angle;
            baboon->maximumTurn = 0;

            if (abs(info.angle) >= BABOON_ATTACK_ANGLE)
            {
                if (info.angle >= 0)
                    item->pos.yRot += BABOON_ATTACK_ANGLE;
                else
                    item->pos.yRot -= BABOON_ATTACK_ANGLE;
            }
            else
            {
                item->pos.yRot += info.angle;
            }

            if (baboon->flags == 0
            && ((item->touchBits & BABOON_TOUCHBITS)
            ||  (item->touchBits & BABOON_RIGHT_TOUCHBITS)
            ||  (item->touchBits & BABOON_JUMP_TOUCHBITS)))
            {
                LaraItem->hitPoints -= BABOON_DAMAGE;
                LaraItem->hitStatus = TRUE;
                CreatureEffect2(item, &baboonBite, 10, -1, DoBloodSplat);
                baboon->flags = 1;
            }
            break;
        }
    }

    CreatureTilt(item, tilt);
    CreatureJoint(item, 0, head_y);
    CreatureAnimation(itemNumber, angle, tilt);
}

void BaboonSearchCrowbarSwitch(ITEM_INFO* item, short flags)
{
    OBJECT_FOUND found = FoundItem(item, CROWBAR_SWITCH);
    if (found.itemNumber != NO_ITEM)
        item->itemFlags[2] = flags; // detected that crowbar_switch exist in this room !
}

void BaboonRespawnFunction(short itemNumber)
{
    ITEM_INFO* item;
    item = &Items[itemNumber];
    BaboonDieEffect(item);

    if (!CheckRespawnedBaboon(itemNumber))
        return;
    UpdateRespawnedBaboon(itemNumber);
}

void BaboonDieEffect(ITEM_INFO* item)
{
    item->pos.yPos -= 128;
    TriggerBaboonShockwave(&item->pos, 0x2000280, -0x30, 0x28802000, ANGLE(0));
    TriggerBaboonShockwave(&item->pos, 0x2000280, -0x30, 0x28802000, ANGLE(45));
    TriggerBaboonShockwave(&item->pos, 0x2000280, -0x30, 0x28802000, ANGLE(90));
    TriggerBaboonShockwave(&item->pos, 0x2000280, -0x30, 0x28802000, ANGLE(135));
    item->pos.yPos += 128;
    FlashRed = 255;
    FlashGreen = 64;
    FlashBlue = 0;
    FlashTime = 32;
}

void BABOON_RESPAWN::Free(void)
{
    baboonRespawnArray.clear();
}

void BABOON_RESPAWN::Add(ITEM_INFO* item, int max_count)
{
    BABOON_RESPAWN_STRUCT toAdd;
    toAdd.id = GetBaboonFreePlace();
    toAdd.pos = item->pos;
    toAdd.count = 0;
    toAdd.max_count = max_count;
    item->itemFlags[0] = toAdd.id; // conserve the id of baboon respawn position on the array...
    baboonRespawnArray.push_back(toAdd);
}

void BABOON_RESPAWN::Remove(int id)
{
    if (baboonRespawnArray.empty())
        return;

    for (auto i = baboonRespawnArray.begin(); i != baboonRespawnArray.end(); i++)
    {
        if (i->id == id)
            baboonRespawnArray.erase(i);
    }
}

int BABOON_RESPAWN::GetBaboonFreePlace()
{
    if (baboonRespawnArray.empty())
        return 0;

    int j = 0;
    for (auto i = baboonRespawnArray.begin(); i != baboonRespawnArray.end(); i++, j++)
    {
        if (i->id == NO_BABOON)
            return j;
    }

    return NO_BABOON;
}

BABOON_RESPAWN_STRUCT* BABOON_RESPAWN::GetBaboonRespawn(int id)
{
    if (baboonRespawnArray.empty())
        return nullptr;
    
    for (auto i = baboonRespawnArray.begin(); i != baboonRespawnArray.end(); i++)
    {
        if (i->id == id)
            return &*i;
    }

    return nullptr;
}

int BABOON_RESPAWN::GetCount(int id)
{
    if (baboonRespawnArray.empty())
        return NO_BABOON_COUNT;

    for (auto i = baboonRespawnArray.begin(); i != baboonRespawnArray.end(); i++)
    {
        if (i->id == id)
            return i->count;
    }

    return NO_BABOON_COUNT;
}

int BABOON_RESPAWN::GetCountMax(int id)
{
    if (baboonRespawnArray.empty())
        return NO_BABOON_COUNT;

    for (auto i = baboonRespawnArray.begin(); i != baboonRespawnArray.end(); i++)
    {
        if (i->id == id)
            return i->max_count;
    }

    return NO_BABOON_COUNT;
}

#ifdef DLL_INJECT
void injector::f_game::inject_baboon()
{
    inject(0x00401DB0, InitialiseBaboon);
    inject(0x00401E50, BaboonControl);
    inject(0x004027E0, BaboonSearchCrowbarSwitch);
    inject(0x00402850, BaboonRespawnFunction);
    inject(0x00402970, BaboonDieEffect);
}
#endif
