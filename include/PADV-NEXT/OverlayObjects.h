#ifndef PADV_OBJECTS
#define PADV_OBJECTS
#include "PADV-NEXT/defines.h"
#include <game/mLib/m_effect.hpp>
extern int FrameTimer;

class dBlower_c {
    private:
    mVec3_c mPos;
    mVec2_c mScale;
    mVec4_c mRect; //To store generateRect(), avoiding repeat calls
    mVec2_c mIntensity;
    mVec2_c mFrameIntensity;
    bool mIsDirty;
    int mAssignedSlot;
    mEf::effect_c mEff;
    
    static const char* getEffectName();
    mVec4_c genAndStoreRect();
    bool CheckOverlap(mVec2_c pos);
    void ApplyForce(dActor_c* obj[]);
    public:
    dBlower_c(mVec3_c pos, mVec2_c scale, mVec2_c intensity);
    ~dBlower_c();
    static void createFromParam(float x, float y, u32 mParam);
    void Execute();
    bool Verify();

    static const int Capacity = 8;
    bool mDelayedDelete;
    static const int MaxInstances = 16;
};
#endif