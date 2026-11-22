#ifndef PADV_STRUCTS
#define PADV_STRUCTS
#include "PADV-NEXT/defines.h"
#include "PADV-NEXT/Utils.h"

extern void* DAT_NORMALCOLLISION;

extern "C" bool isPause();
extern dAcPy_c* Players[4];
extern int FindPointerInArray(void *arr[], int size, void *ptr);
extern "C" void SetConveyerSpeed(dAcPy_c*, float);
extern "C" int *checkGrounded(dAcPy_c*);

enum ExecPhase
{
    NONE = 0,
    PRE = 1,
    POST = 2,
    ALL = 3
};

enum ActorLayer
{
    NORMAL = 0,
    FOREGROUND = 1,
    BACKGROUND = 2
};

//Basically just my own names for the BcPoint, because that means i can keep the normal headers.
struct TileCollider {
    int mFlags;
    int mLineA;
    int mLineB;
    int mDistance;
    static const int Divider = 2048;

    //Assumes *2048 scaled numbers
    static TileCollider CreatePrescaled(int Flags, int LineA, int LineB, int Distance) {return TileCollider(Flags, LineA, LineB, Distance); }

    //Assumes unscaled (*1) numbers, scales automatically
    static TileCollider CreateUnscaled(int Flags, int LineA, int LineB, int Distance) { return CreatePrescaled(Flags, LineA * Divider, LineB * Divider, Distance); }

    TileCollider(int Flags, int LineA, int LineB, int Distance) { mFlags = Flags; mLineA = LineA; mLineB = LineB; mDistance = Distance; }
    ~TileCollider() {}
};

struct TileCollisionSet {
    TileCollider mFoot;
    TileCollider mHead;
    TileCollider mWall;
    TileCollider mVine;
    TileCollisionSet(TileCollider Foot, TileCollider Head, TileCollider Wall, TileCollider Vine) : mFoot(Foot), mHead(Head), mWall(Wall), mVine(Vine) {}
    TileCollisionSet(int FlagC[4], int LineAC[4], int LineBC[4], int DistanceC[4]) : 
        mFoot(TileCollider::CreatePrescaled(FlagC[0], LineAC[0], LineBC[0], DistanceC[0])),
        mHead(TileCollider::CreatePrescaled(FlagC[1], LineAC[1], LineBC[1], DistanceC[1])),
        mWall(TileCollider::CreatePrescaled(FlagC[2], LineAC[2], LineBC[2], DistanceC[2])),
        mVine(TileCollider::CreatePrescaled(FlagC[3], LineAC[3], LineBC[3], DistanceC[3]))
        {}

    ~TileCollisionSet() {}
};

struct TileCollisionOverride {
    TileCollisionSet mNormal;
    TileCollisionSet mCrouch;
    TileCollisionSet mUnk1;
    TileCollisionSet mUnk2;
    TileCollisionSet mStar;
    dAcPy_c* mOwner; //This should be out of range of the original code, so that it wont push this into
    static const TileCollisionOverride* DefaultBase() { return (const TileCollisionOverride*)&DAT_NORMALCOLLISION; }

    //DO NOT RUN THIS MORE THAN ONCE PER FRAME
    static TileCollisionOverride CreateMultiplied(float XMulti, float YMulti) {
        TileCollisionOverride out = TileCollisionOverride(*DefaultBase());
        out.mNormal.mFoot.mLineA *= XMulti;
        out.mNormal.mFoot.mLineB *= XMulti;
        out.mNormal.mFoot.mDistance *= YMulti;
        out.mNormal.mHead.mLineA *= XMulti;
        out.mNormal.mHead.mLineB *= XMulti;
        out.mNormal.mHead.mDistance *= YMulti;
        out.mNormal.mWall.mLineA *= YMulti;
        out.mNormal.mWall.mLineB *= YMulti;
        out.mNormal.mWall.mDistance *= XMulti;
        out.mNormal.mVine.mLineA *= YMulti;
        out.mNormal.mVine.mLineB *= YMulti;
        out.mNormal.mVine.mDistance *= XMulti;

        out.mCrouch.mFoot.mLineA *= XMulti;
        out.mCrouch.mFoot.mLineB *= XMulti;
        out.mCrouch.mFoot.mDistance *= YMulti;
        out.mCrouch.mHead.mLineA *= XMulti;
        out.mCrouch.mHead.mLineB *= XMulti;
        out.mCrouch.mHead.mDistance *= YMulti;
        out.mCrouch.mWall.mLineA *= YMulti;
        out.mCrouch.mWall.mLineB *= YMulti;
        out.mCrouch.mWall.mDistance *= XMulti;
        out.mCrouch.mVine.mLineA *= YMulti;
        out.mCrouch.mVine.mLineB *= YMulti;
        out.mCrouch.mVine.mDistance *= XMulti;

        out.mUnk1.mFoot.mLineA *= XMulti;
        out.mUnk1.mFoot.mLineB *= XMulti;
        out.mUnk1.mFoot.mDistance *= YMulti;
        out.mUnk1.mHead.mLineA *= XMulti;
        out.mUnk1.mHead.mLineB *= XMulti;
        out.mUnk1.mHead.mDistance *= YMulti;
        out.mUnk1.mWall.mLineA *= YMulti;
        out.mUnk1.mWall.mLineB *= YMulti;
        out.mUnk1.mWall.mDistance *= XMulti;
        out.mUnk1.mVine.mLineA *= YMulti;
        out.mUnk1.mVine.mLineB *= YMulti;
        out.mUnk1.mVine.mDistance *= XMulti;

        out.mUnk2.mFoot.mLineA *= XMulti;
        out.mUnk2.mFoot.mLineB *= XMulti;
        out.mUnk2.mFoot.mDistance *= YMulti;
        out.mUnk2.mHead.mLineA *= XMulti;
        out.mUnk2.mHead.mLineB *= XMulti;
        out.mUnk2.mHead.mDistance *= YMulti;
        out.mUnk2.mWall.mLineA *= YMulti;
        out.mUnk2.mWall.mLineB *= YMulti;
        out.mUnk2.mWall.mDistance *= XMulti;
        out.mUnk2.mVine.mLineA *= YMulti;
        out.mUnk2.mVine.mLineB *= YMulti;
        out.mUnk2.mVine.mDistance *= XMulti;

        out.mStar.mFoot.mLineA *= XMulti;
        out.mStar.mFoot.mLineB *= XMulti;
        out.mStar.mFoot.mDistance *= YMulti;
        out.mStar.mHead.mLineA *= XMulti;
        out.mStar.mHead.mLineB *= XMulti;
        out.mStar.mHead.mDistance *= YMulti;
        out.mStar.mWall.mLineA *= YMulti;
        out.mStar.mWall.mLineB *= YMulti;
        out.mStar.mWall.mDistance *= XMulti;
        out.mStar.mVine.mLineA *= YMulti;
        out.mStar.mVine.mLineB *= YMulti;
        out.mStar.mVine.mDistance *= XMulti;
        return out;
    }

    TileCollisionOverride(TileCollisionSet Normal, TileCollisionSet Crouch, TileCollisionSet Unk1, TileCollisionSet Unk2, TileCollisionSet Star) : mNormal(Normal), mCrouch(Crouch), mUnk1(Unk1), mUnk2(Unk2), mStar(Star) {}
    TileCollisionOverride(const TileCollisionOverride& other) : mNormal(other.mNormal), mCrouch(other.mCrouch), mUnk1(other.mUnk1), mUnk2(other.mUnk2), mStar(other.mStar) {}
    TileCollisionOverride(int FlagC[20], int LineAC[20], int LineBC[20], int DistanceC[20]) : 
    mNormal(FlagC, LineAC, LineBC, DistanceC),
    mCrouch(FlagC + 4, LineAC + 4, LineBC + 4, DistanceC + 4),
    mUnk1(FlagC + 8, LineAC + 8, LineBC + 8, DistanceC + 8),
    mUnk2(FlagC + 12, LineAC + 12, LineBC + 12, DistanceC + 12),
    mStar(FlagC + 16, LineAC + 16, LineBC + 16, DistanceC + 16)
    {}
    ~TileCollisionOverride() {}
};

// Level and World are 1-Indexed. Do not enter 0 unless it shouldnt ever work.
struct Modifier
{
    typedef void (*ModFunc)(void *);
    typedef void (*ModFuncNoArg)();

public:
    const u8 Level;
    const u8 World;
    const u8 Area; // 0 for All
    const ExecPhase Phase;
    const ModFunc Mod;
    const ModFuncNoArg ModNoArg;
    int Timer; // 0 for None
    void *arg;
    const bool useArg;
    const bool shouldRunWhilePaused;

    void TryRun(ExecPhase phase) const
    {
        dScStage_c *stage = dScStage_c::getInstance();
        if (!stage || (!Mod && !ModNoArg))
            return;
        if (Phase == NONE || phase != Phase && Phase != ALL)
            return;
        if (Level == 0 || World == 0)
            goto special;
        else if (Level != stage->mCurrCourse + 1 || World != stage->mCurrWorld + 1)
            return;
        if (Area != 0 && Area != stage->mCurrAreaNo + 1)
            return;
    special:
        if (!shouldRunWhilePaused && isPause())
            return;
        if (Timer == 0)
        {
            if (!useArg)
            {
                this->ModNoArg();
            }
            else
            {
                this->Mod(arg);
            }
        }
        else if (CallSpacer(Timer))
        {
            if (!useArg)
            {
                this->ModNoArg();
            }
            else
            {
                this->Mod(arg);
            }
        }
    }

    Modifier(u8 level, u8 world, u8 area, ExecPhase phase, void(func)(void *), int timer, void *argument, bool UseArg, bool runWhilePause) : Level(level), World(world), Area(area), Phase(phase), Mod((ModFunc)func), ModNoArg((ModFuncNoArg)func), useArg(UseArg), shouldRunWhilePaused(runWhilePause)
    {
        Timer = timer;
        arg = argument;
    }
};

typedef struct mVec4_c
{
    float x;
    float y;
    float z;
    float w;

    bool operator==(const mVec4_c &other) const
    {
        return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
    }

    mVec4_c& operator+=(const mVec4_c &other)
    {
        this->x += other.x; 
        this->y += other.y;
        this->z += other.z; 
        this->w += other.w;
        return *this;
    }

    mVec4_c& operator-=(const mVec4_c &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }

    mVec4_c& operator*=(const mVec4_c &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        this->w *= other.w;
        return *this;
    }

    mVec4_c& operator*=(const float other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        this->w *= other;
        return *this;
    }

    mVec4_c& operator/=(const mVec4_c &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        this->w /= other.w;
        return *this;
    }

    mVec4_c& operator/=(const float other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        this->w /= other;
        return *this;
    }

    mVec4_c()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    mVec4_c(float X, float Y, float Z, float W)
    {
        x = X;
        y = Y;
        z = Z;
        w = W;
    }

    mVec4_c(mVec2_c a, mVec2_c b)
    {
        x = a.x;
        y = a.y;
        z = b.x;
        w = b.y;
    }

    ~mVec4_c()
    {
        this->x = NULL;
        this->y = NULL;
        this->z = NULL;
        this->w = NULL;
    }

} Rect2D;

typedef struct mVecU16_c
{
    u16 x;
    u16 y;

    bool operator==(const mVecU16_c &other) const
    {
        return (this->x == other.x && this->y == other.y);
    }

    mVecU16_c& operator+=(const mVecU16_c &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    mVecU16_c& operator-=(const mVecU16_c &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    mVecU16_c& operator*=(const mVecU16_c &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }

    mVecU16_c& operator*=(const u16 other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    mVecU16_c& operator/=(const mVecU16_c &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }

    mVecU16_c& operator/=(const u16 other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    mVecU16_c()
    {
        x = 0;
        y = 0;
    }

    mVecU16_c(u16 X, u16 Y)
    {
        x = X;
        y = Y;
    }

    ~mVecU16_c()
    {
        this->x = NULL;
        this->y = NULL;
    }
};

typedef struct mVecInt_c 
{
    u32 x;
    u32 y;
    u32 z;

    bool operator==(const mVecInt_c &other) const
    {
        return (this->x == other.x && this->y == other.y && this->z == other.z);
    }

    mVecInt_c& operator+=(const mVecInt_c &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    mVecInt_c& operator-=(const mVecInt_c &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    mVecInt_c& operator*=(const mVecInt_c &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }

    mVecInt_c& operator*=(const u32 other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }

    mVecInt_c& operator/=(const mVecInt_c &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

    mVecInt_c& operator/=(const u32 other)
    {
        this->x += other;
        this->y += other;
        this->z += other;
        return *this;
    }

    mVecInt_c()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    mVecInt_c(u32 X, u32 Y, u32 Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    ~mVecInt_c()
    {
        this->x = NULL;
        this->y = NULL;
        this->z = NULL;
    }
};

struct PhysicsOverride {
    public:
    dAcPy_c* mOwner;
    mVec2_c mCCOffset; //Normal +/- for XY, From Center
    mVec2_c mCCScale; //Mirrored from Center by Vanilla
    const TileCollisionOverride CollisionOverride;
    
    float mBgConveyerbeltness;
    bool State[5];
    void** arr;

    void Apply() {
        if(mOwner < (dAcPy_c*)0x80000000) return;
        if(State[1]) {
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mCc, 0x24) = mCCScale;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mCc1, 0x24) = mCCScale;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc1, 0x24) = mCCScale;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc2, 0x24) = mCCScale;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc3, 0x24) = mCCScale;
        }
        if(State[2]) {
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mCc, 0x1C) = mCCOffset;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mCc1, 0x1C) = mCCOffset;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc1, 0x1C) = mCCOffset;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc2, 0x1C) = mCCOffset;
            *(mVec2_c*)GetMemberFromOffset(&mOwner->mAttCc3, 0x1C) = mCCOffset;
        }
        if(State[4] && (*checkGrounded(mOwner) & 0x0000000F) == 1) SetConveyerSpeed(mOwner, mBgConveyerbeltness);
        //The Others arent Implemented yet lol
        if(!State[0]) delete this;
    }

    PhysicsOverride(dAcPy_c* owner, mVec2_c CCScale, mVec2_c CCOffset, mVec2_c TileMultiplier, float conveyerness, bool keepAlive, void** array) : CollisionOverride(TileCollisionOverride::CreateMultiplied(TileMultiplier.x, TileMultiplier.y)) {
        mOwner = owner;
        mCCScale = CCScale; 
        mCCOffset = CCOffset; 
        mBgConveyerbeltness = conveyerness;
        arr = array;
        State[0] = keepAlive; //Keep Alive Until Level Reload.
        State[1] = CCScale.x || CCScale.y;
        State[2] = CCOffset.x || CCOffset.y;
        State[3] = TileMultiplier.x && TileMultiplier.y; //Deferred into OnBcPickSource
        State[4] = conveyerness;
        
        int id = FindPointerInArray((void**)Players, 4, owner);
        if(id == 0xFFFF) OSReport("Owner not found, memory leak achieved\n");
        else {
            array[id] = this;
        }
    }

    ~PhysicsOverride() {
        int id = FindPointerInArray(arr, 4, this);
        if(id != 0xFFFF) arr[id] = NULL;
    };
};


enum Powers
{
    POWER_SMALL = 0,
    POWER_BIG = 1,
    POWER_FIRE = 2,
    POWER_MINI = 3,
    POWER_PROPELLER = 4,
    POWER_PENGUIN = 5,
    POWER_ICE = 6
};

// DEATHMUSH is not actually in this game, but im going to change that.
// If you spawn DEATHMUSH in the vanilla game, you get a normal-mush-scaled Minimush, which acts like a minimush.
// Time for me to find what function occours when you get a powerup.
enum ItemVariants
{
    ITEM_MUSHROOM = 0x00,
    ITEM_STAR = 0x01,
    ITEM_ONEUP = 0x07,
    ITEM_FIRE = 0x09,
    ITEM_ICE = 0x0E,
    ITEM_PENGUIN = 0x11,
    ITEM_PROPELLER = 0x15,
    ITEM_MINI = 0x19,
    ITEM_DEATHMUSH = 0xF9
};

enum ExitMode
{
    EXIT_SUCCESS = 0,
    EXIT_FAIL = 1,
    EXIT_PLAYER_CHOICE = 2,
    EXIT_DEFAULT = 3
};

enum GameMode
{
    GAME_NORMAL = 0,
    GAME_LUIGI_GUIDE = 1,
    GAME_TITLE = 2,
    GAME_RETURN_ON_INPUT = 3,
    GAME_MOVIE = 4 // 1-41's Hint Movies
};

enum Minigame
{
    MINIGAME_NONE = 0,
    MINIGAME_STAR_HOUSE = 1,
    MINIGAME_TOAD_HOUSE = 2
};

// If i ever need to overwrite a class, here's the ones that do nothing
// dDummyDoorParent_c and dDummyDoorChild_c are not included, as they execute... just ... unknown things.. They dont even need eachother btw
enum Dummy
{
    SLOW_QUICK_TAG_C = (int)SLOW_QUICK_TAG,
    KAWANAGARE_C = (int)KAWANAGARE,
    HANA_MOUNTAIN_C = (int)HANA_MOUNTAIN,
    TAG_THUNDER_C = (int)TAG_THUNDER,
    BRANCH_C = (int)BRANCH,
    AC_LIFT_ICE_SPRING_C = (int)AC_LIFT_ICE_SPRING,
    EN_BLUR_C = (int)EN_BLUR
};

//Crashes afaik
enum Broken
{
    AC_LIFT_OBJBG_HMOVE_BIG_C = (int)AC_LIFT_OBJBG_HMOVE_BIG
};
 
#endif
