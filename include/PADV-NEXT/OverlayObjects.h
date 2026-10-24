#ifndef PADV_OBJECTS
#define PADV_OBJECTS
#include "PADV-NEXT/defines.h"
#include <game/mLib/m_effect.hpp>
extern int FrameTimer;

class OverlayObjectBase {
    protected:
    const u16 OVERLAYED_ACTOR;
    mVec2_c mPos;
    mVec2_c mScale;
    int mAssignedSlot;

    public:
    bool mDelayedDelete;
    virtual void Execute();
    virtual bool Verify();
    OverlayObjectBase(mVec2_c pos, mVec2_c scale, u16 OverlayedActorID);
    virtual ~OverlayObjectBase();
};

class dDetectionField_c : public OverlayObjectBase {
    protected:
    bool mIsDirty;
    mVec4_c mRect;

    virtual bool CheckOverlap(mVec2_c pos);
    virtual mVec4_c genAndStoreRect();

    public:
    virtual void Apply(dActor_c* obj[]);
    void Execute();
    bool Verify();

    dDetectionField_c(mVec2_c pos, mVec2_c scale, u16 OverlayedActorID);
    ~dDetectionField_c();
};

class dBlower_c : public dDetectionField_c {
    private:
    mVec2_c mIntensity;
    mVec2_c mFrameIntensity;
    mEf::effect_c mEff;
    
    static const char* getEffectName();
    void Apply(dActor_c* obj[]);
    bool CheckOverlap(mVec2_c pos);
    public:
    static void createFromParam(float x, float y, u32 mParam, u16 OverlayedActorID);
    bool Verify();
    void Execute();
    mVec4_c genAndStoreRect();

    dBlower_c(mVec3_c pos, mVec2_c scale, mVec2_c intensity, u16 OverlayedActorID);
    ~dBlower_c();

    static const int Capacity = 8;
};

//finish tomorrow
class dPlayerModifier_c : public dDetectionField_c {
    private:
    mEf::effect_c LUC;
    mEf::effect_c LDC;
    mEf::effect_c RUC;
    mEf::effect_c RDC;
    dActor_c* mActorBuffer[4];
    dActor_c* mNewActorsBuffer[4];

    Actors mSpawn;
    Powers mPowerup;
    LAYER_e mLayer;
    bool mForce;
    u32 mSpawnSettings;
    
    bool CheckOverlap(mVec2_c pos);
    mVec4_c genAndStoreRect();
    void Apply(dActor_c* obj[]); 

    static const char* getEffectName();

    public:
    bool Verify();
    void Execute();

    static void createFromParam(float x, float y, u32 param, u16 OverlayedActorID);
    dPlayerModifier_c(float x, float y, u8 xs, u8 ys, u16 OverlayedActorID);
    ~dPlayerModifier_c();
};
#endif