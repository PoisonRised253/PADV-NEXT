#include "PADV-NEXT/OverlayObjects.h"
#include "PADV-NEXT/math.hpp"
extern OverlayObjectBase *OverlayObjects[MAX_OVERLAYOBJECTS];

OverlayObjectBase::OverlayObjectBase(mVec2_c pos, mVec2_c scale, u16 OverlayedActorID) : OVERLAYED_ACTOR(OverlayedActorID)
{
    mPos = pos;
    mScale = scale;
    mDelayedDelete = false;
    int freeSlot = FindNextFreeArrayEntry((void **)OverlayObjects, MAX_OVERLAYOBJECTS);
    if (freeSlot != 0xFFFF)
    {
        mAssignedSlot = freeSlot;
        OverlayObjects[freeSlot] = this;
    }
    else
        mAssignedSlot = -1;
}

OverlayObjectBase::~OverlayObjectBase()
{
    if (mAssignedSlot != -1)
        OverlayObjects[mAssignedSlot] = NULL;
}

bool OverlayObjectBase::Verify()
{
    if (mAssignedSlot == -1 || mDelayedDelete || !isInStage)
    {
        delete this;
        return false;
    }
    return true;
}

dDetectionField_c::dDetectionField_c(mVec2_c pos, mVec2_c scale, u16 OverlayedActorID) : OverlayObjectBase(pos, scale, OverlayedActorID)
{
    mIsDirty = true;
}

mVec4_c dDetectionField_c::genAndStoreRect()
{
    mVec4_c out = mVec4_c(
        mPos.x - (mScale.x / 2),
        mPos.y - (mScale.y / 2),
        mPos.x + (mScale.x / 2),
        mPos.y + (mScale.y / 2));
    mRect = out;

    return out;
}

bool dDetectionField_c::CheckOverlap(mVec2_c pos)
{
    static const mVec4_c Zero = mVec4_c(0, 0, 0, 0);
    if (mRect == Zero || mIsDirty)
        genAndStoreRect();
    return pos.x >= mRect.x &&
           pos.x <= mRect.z &&
           pos.y >= mRect.y &&
           pos.y <= mRect.w;
}

// mVec2_c Rect is now *32 scaled, to avoid manual math lol (32 cuz both sides)
dBlower_c::dBlower_c(mVec3_c pos, mVec2_c rect, mVec2_c intensity, u16 OverlayedActorID) : dDetectionField_c(mVec2_c(pos.x, pos.y), mVec2_c(rect.x * 32, rect.y * 32), OverlayedActorID), mEff(mEf::effect_c())
{
    mIntensity = intensity;
    static const float effectScaleFix = 0.15625f;
    mVec3_c effectScale = mVec3_c(rect.x + (effectScaleFix * rect.x), rect.y, 1);
    mVec3_c effectPos = mVec3_c(mPos.x, mPos.y - (mScale.y / 2), -6500.f);
    mEff.createEffect(getEffectName(), 0, &effectPos, &mAng3_c::Zero, &effectScale);
}

void dBlower_c::Execute()
{
    dActor_c *forceBuff[Capacity] = {NULL};
    for (int i = 0; i < 4; i++)
    {
        if (!Players[i])
            continue;
        if (CheckOverlap(MakeVec2(Players[i]->mPos.x, Players[i]->mPos.y)))
        {
            int freeSlot = FindNextFreeArrayEntry((void **)forceBuff, Capacity);
            if (freeSlot != 0xFFFF)
                forceBuff[freeSlot] = (dActor_c *)Players[i];
        }
    }
    Apply(forceBuff);
}

// Generates mVec2_c BL, mVec2_c TR and smashes them together into one struct
mVec4_c dBlower_c::genAndStoreRect()
{
    mFrameIntensity = mIntensity / 60;
    mVec4_c out = mVec4_c(
        mPos.x - (mScale.x / 2),
        mPos.y - ((mScale.y / 2) * 1.5f),
        mPos.x + (mScale.x / 2),
        mPos.y + ((mScale.y / 2) * 1.95f));
    mRect = out;

    return out;
}

void dBlower_c::Apply(dActor_c *obj[])
{
    if (!obj[0] || !isInStage)
        return;
    for (int i = 0; i < Capacity; i++)
    {
        bool isPlayer = false;
        if (!obj[i])
            return; // Since the Array fills from 0 onward without any spaces inbetween, just fuckin cancel shit if there is no more in the next slot
        // It appears i am not able to clamp vectors. I should really fix that sometime
        if (i < 4 && obj[i] == Players[i])
        {
            isPlayer = true;
            if (Players[i]->mStateMgr.getStateID()->isEqual(dAcPy_c::StateID_SpinJump) || Players[i]->mStateMgr.getStateID()->isEqual(dAcPy_c::StateID_DemoFallDown))
                continue;
        }

        mVec2_c calc = mVec2_c(
            clampSymmetric(obj[i]->mSpeedF + mFrameIntensity.x, mIntensity.x),
            clampSymmetric(obj[i]->mSpeed.y + mFrameIntensity.y, mIntensity.y));
        if (isPlayer && CompareAgainstInput(WPAD_BUTTON_2, i))
        {
            clampSymmetric(obj[i]->mSpeedF + (mFrameIntensity.x / 8), mIntensity.x),
                calc.y = clampSymmetric(obj[i]->mSpeed.y + mFrameIntensity.y, mIntensity.y / 3);
        }
        OSReport("ID=%i,FX=%f,FY=%f\n", mAssignedSlot, calc.x, calc.y);
        if (mIntensity.x != 0)
            obj[i]->mSpeedF = calc.x;
        if (mIntensity.y != 0)
            obj[i]->mSpeed.y = calc.y;
        if (isPlayer && Players[i]->mStateMgr.getStateID()->isEqual(dAcPy_c::StateID_HipAttack))
            obj[i]->mSpeedF = 0;
    }
    return;
}

void dBlower_c::createFromParam(float x, float y, u32 mParam, u16 OverlayedActorID)
{
    if (x == 0 || y == 0)
        return;
    char xs = (mParam & 0xFF000000) >> 24, ys = (mParam & 0x00FF0000) >> 16;
    char signX = ((mParam & 0x0000F000) >> 12), signY = ((mParam & 0x000000F0) >> 4);
    float intenX = (float)((mParam & 0x00000F00) >> 8), intenY = (float)(mParam & 0x0000000F);
    if (signX > 0)
        intenX = -intenX;
    if (signY > 0)
        intenY = -intenY;

    new dBlower_c(mVec3_c(x, y, 0), mVec2_c(abs(xs), abs(ys)), mVec2_c(intenX, intenY), OverlayedActorID);
}

void dPlayerModifier_c::Execute()
{
    mNewActorsBuffer[0] = NULL;
    mNewActorsBuffer[1] = NULL;
    mNewActorsBuffer[2] = NULL;
    mNewActorsBuffer[3] = NULL;

    for (int i = 0; i < 4; i++)
    {
        if (!Players[i])
            continue;
        
        int position = FindPointerInArray((void **)mActorBuffer, 4, Players[i]);

        int nextFreeSlotNew = FindNextFreeArrayEntry((void **)mNewActorsBuffer, 4);
        int nextFreeSlotMain = FindNextFreeArrayEntry((void **)mActorBuffer, 4);

        bool isInTheRange = CheckOverlap(mVec2_c(Players[i]->mPos.x, Players[i]->mPos.y));

        if (isInTheRange)
        {
            if (position == 0xFFFF)
            {
                if (nextFreeSlotNew != 0xFFFF)
                    mNewActorsBuffer[nextFreeSlotNew] = Players[i];
            }
            if (nextFreeSlotMain != 0xFFFF)
                mActorBuffer[nextFreeSlotMain] = Players[i];
        }
        else if(position != 0xFFFF)
        {
            mActorBuffer[position] = NULL;
        }
    }
    Apply(mNewActorsBuffer);
}

void dPlayerModifier_c::Apply(dActor_c *obj[])
{
    for (int i = 0; i < 4; i++)
    {
        if (mActorBuffer[i] && mForce)
        {
            *GetPlayerPowerState((dAcPy_c *)mActorBuffer[i]) = mPowerup;
            SetActorCollisionLayer(mActorBuffer[i], mLayer);
        }
        if (obj[i])
        {
            *GetPlayerPowerState((dAcPy_c *)obj[i]) = mPowerup;
            SetActorCollisionLayer(obj[i], mLayer);
        }
    }
}

void dPlayerModifier_c::createFromParam(float x, float y, u32 param, u16 OverlayedActorID)
{
    char xs = ((param & 0xF0000000) >> 28), ys = ((param & 0x0F000000) >> 24);
    x -= 8;
    dPlayerModifier_c *pMod = new dPlayerModifier_c(x, y, xs, ys, OverlayedActorID);

    float yst = (ys * 32) / 2;
    if(pMod->OVERLAYED_ACTOR == (u16)DUMMY_DOOR_CHILD)
        yst += 24;
    y += yst;

    char actorSelect = ((param & 0x00F00000) >> 20), powerSelect = ((param & 0x000F0000) >> 16), layerSelect = ((param & 0x0000F000) >> 12);
    bool force = (param & 0x0000000F);
    pMod->mForce = force;
    pMod->mLayer = (LAYER_e)layerSelect;
    pMod->mPowerup = (Powers)powerSelect;

    //Non-Implemented
    switch (actorSelect)
    {
    case 1:
        pMod->mSpawn = EN_ITEM;
        pMod->mSpawnSettings = 0x000000F9;
        break;
    case 2:
        pMod->mSpawn = YOSHI;
        pMod->mSpawnSettings = 0x0;
        break;
    case 3:
        pMod->mSpawn = BROS_FIREBALL;
        pMod->mSpawnSettings = 0x0;
        break;

    default:
        pMod->mSpawn = (Actors)-1;
    }
}

dPlayerModifier_c::dPlayerModifier_c(float x, float y, u8 xs, u8 ys, u16 OverlayedActorID) : dDetectionField_c(mVec2_c(x, y), mVec2_c(xs * 32, ys * 32), OverlayedActorID), LUC(), RUC(), LDC(), RDC()
{
    mIsDirty = true;
    mActorBuffer[0] = NULL;
    mActorBuffer[1] = NULL;
    mActorBuffer[2] = NULL;
    mActorBuffer[3] = NULL;
}

//This line of code, makes a beautifiul effect appear, use with CallSpacer(30)
//SpawnEffect("Wm_bs_kameckmagic", 0, &this->pos, &r, &MakeVec(this->aoeX / 32, this->aoeY / 64, 1));
mVec4_c dPlayerModifier_c::genAndStoreRect()
{
    mVec4_c out = dDetectionField_c::genAndStoreRect();
    static const mVec3_c one = mVec3_c(1,1,1);

    LUC.createEffect(getEffectName(), 0, &mVec3_c(out.x, out.y, 9000), &mAng3_c::Zero, &one);
    LDC.createEffect(getEffectName(), 0, &mVec3_c(out.x, out.w, 9000), &mAng3_c::Zero, &one);
    RUC.createEffect(getEffectName(), 0, &mVec3_c(out.z, out.y, 9000), &mAng3_c::Zero, &one);
    RDC.createEffect(getEffectName(), 0, &mVec3_c(out.z, out.w, 9000), &mAng3_c::Zero, &one);
    return out;
}

// Literal Inlines
void OverlayObjectBase::Execute() { this->Verify(); }

void dDetectionField_c::Execute() { this->Verify(); }
void dDetectionField_c::Apply(dActor_c *obj[]) {}
bool dDetectionField_c::Verify() { return OverlayObjectBase::Verify(); }
dDetectionField_c::~dDetectionField_c() {}

bool dBlower_c::Verify() { return dDetectionField_c::Verify(); }
bool dBlower_c::CheckOverlap(mVec2_c pos) { return dDetectionField_c::CheckOverlap(pos); }
const char *dBlower_c::getEffectName() { return "Wm_ob_stream"; }
dBlower_c::~dBlower_c() {}

bool dPlayerModifier_c::Verify() { return dDetectionField_c::Verify(); }
bool dPlayerModifier_c::CheckOverlap(mVec2_c pos) { return dDetectionField_c::CheckOverlap(pos); }
const char *dPlayerModifier_c::getEffectName() { return "Wm_en_bubble"; }
dPlayerModifier_c::~dPlayerModifier_c() {}