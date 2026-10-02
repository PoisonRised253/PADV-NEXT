#include "PADV-NEXT/OverlayObjects.h"
#include "PADV-NEXT/math.hpp"
extern dBlower_c* Blowers[dBlower_c::MaxInstances];

//mVec2_c Rect is now *32 scaled, to avoid manual math lol (32 cuz both sides)
dBlower_c::dBlower_c(mVec3_c pos, mVec2_c rect, mVec2_c intensity) : mEff(mEf::effect_c()) {
    mPos = pos;
    mScale = mVec2_c(rect.x * 32, rect.y * 32);
    mIntensity = intensity;
    mIsDirty = true;
    mDelayedDelete = false;
    static const float effectScaleFix = 0.15625f;
    mVec3_c effectScale = mVec3_c(rect.x + (effectScaleFix * rect.x), rect.y, 1);
    mVec3_c effectPos = mVec3_c(mPos.x, mPos.y - (mScale.y / 2), -6500.f);
    mEff.createEffect(getEffectName(), 0, &effectPos, &mAng3_c::Zero, &effectScale);
    int freeSlot = FindNextFreeArrayEntry((void**)Blowers, Capacity);
    OSReport("freeSlot == %i\n", freeSlot);
    if(freeSlot != 0xFFFF) {
        mAssignedSlot = freeSlot;
        Blowers[freeSlot] = this;
    }
    else mAssignedSlot = -1;
}

dBlower_c::~dBlower_c() {
    if(mAssignedSlot != -1)
        Blowers[mAssignedSlot] = NULL;
}

void dBlower_c::Execute() {
    dActor_c* forceBuff[Capacity] = {NULL};
    for(int i = 0; i < 4; i++) {
        if(!Players[i]) continue;
        if(CheckOverlap(MakeVec2(Players[i]->mPos.x, Players[i]->mPos.y))) {
            int freeSlot = FindNextFreeArrayEntry((void**)forceBuff, Capacity);
            if(freeSlot != 0xFFFF) forceBuff[freeSlot] = (dActor_c*)Players[i];
        }
    }
    ApplyForce(forceBuff);
}

//Generates mVec2_c BL, mVec2_c TR and smashes them together into one struct
mVec4_c dBlower_c::genAndStoreRect() {
    mFrameIntensity = mIntensity / 60;
    mVec4_c out = mVec4_c(
        mPos.x - (mScale.x / 2),
        mPos.y - ((mScale.y / 2) * 1.5f),
        mPos.x + (mScale.x / 2),
        mPos.y + ((mScale.y /2) * 1.95f)
    );
    mRect = out;

    return out;
}

bool dBlower_c::CheckOverlap(mVec2_c pos) {
    static const mVec4_c Zero = mVec4_c(0,0,0,0);
    if(mRect == Zero || mIsDirty) genAndStoreRect();
    return pos.x >= mRect.x &&
           pos.x <= mRect.z &&
           pos.y >= mRect.y &&
           pos.y <= mRect.w;
}

void dBlower_c::ApplyForce(dActor_c* obj[]) {
    if(!obj[0] || !isInStage) return;
    for(int i = 0; i < Capacity; i++) {
        bool isPlayer = false;
        if(!obj[i]) return; //Since the Array fills from 0 onward without any spaces inbetween, just fuckin cancel shit if there is no more in the next slot
        //It appears i am not able to clamp vectors. I should really fix that sometime
        if(i < 4 && obj[i] == Players[i]) {
            isPlayer = true;
            if(Players[i]->mStateMgr.getStateID()->isEqual(dAcPy_c::StateID_SpinJump)) continue;
        }

        mVec2_c calc = mVec2_c(
            clampSymmetric(obj[i]->mSpeedF + mFrameIntensity.x, mIntensity.x),
            clampSymmetric(obj[i]->mSpeed.y + mFrameIntensity.y, mIntensity.y)
        );
        if(isPlayer && CompareAgainstInput(WPAD_BUTTON_2, i)) {
            calc.x = Players[i]->mSpeedF;
            calc.y = clampSymmetric(obj[i]->mSpeed.y + mFrameIntensity.y, mIntensity.y / 3);
        }
        if(mIntensity.x != 0) obj[i]->mSpeedF = calc.x;
        if(mIntensity.y != 0) obj[i]->mSpeed.y = calc.y;
    }
    return;
}

bool dBlower_c::Verify() {
    if(mAssignedSlot == -1 || mDelayedDelete) {delete this; return false;}
    return true;
}

const char* dBlower_c::getEffectName() {
    return "Wm_ob_stream";
}

void dBlower_c::createFromParam(float x, float y, u32 mParam) {
    if(x == 0 || y == 0) return;
    u8 xs = (mParam & 0xFF000000) >> 24, ys = (mParam & 0x00FF0000) >> 16;
    char signX = ((mParam & 0x0000F000) >> 12), signY = ((mParam & 0x000000F0) >> 4);
    float intenX = (float)((mParam & 0x00000F00) >> 8), intenY = (float)(mParam & 0x0000000F);
    if(signX > 0) intenX = -intenX;
    if(signY > 0) intenY = -intenY;
   
    new dBlower_c(mVec3_c(x,y,0),mVec2_c(xs,ys),mVec2_c(intenX, intenY));
}