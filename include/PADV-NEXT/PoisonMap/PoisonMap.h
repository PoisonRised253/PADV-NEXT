#ifdef PADV_PMAP_ENABLE
#ifndef PADV_PMAP
#define PADV_PMAP
#include "PADV-NEXT/defines.h"

struct PoisonMapPoint
{
public:
    u8 mID;

    mVecInt_c mPos;
    mAng3_c mRot;
    mVecInt_c mScale;

    u8 mNeighbors[4];
    int mNodeType;

    u16 mLevel, mWorld;

    static PoisonMapPoint* Create(const char* csvStart, const char* csvEnd);

    PoisonMapPoint();
    ~PoisonMapPoint();
private:
    void ParseRawCSV();
    void* ConvertIntToMember(int item);

    char RawCSV[128];
};

struct PoisonMapRoute
{
public:
    PoisonMapPoint *mDirections[4];
    float mTravelTime;
    bool mUnlocked;

private:
    char RawCSV[128];
};

struct PoisonMapData
{
public:
    PoisonMapPoint *mPoints[256];
    PoisonMapRoute *mRoutes[256];
 
    static PoisonMapData* Initialize(int world, int subworld);

    // Returns amount of lines present, creating a range to work with. 0-returnValue
    static nw4r::g3d::ResFile GetMasterCSV(int world);
    void CollectPoints(nw4r::g3d::ResFile* csv);
    void BuildPoints();
    void BuildRoutes();
    void ConvertToVanillaPathSystem();

    PoisonMapData();
    ~PoisonMapData();

private:
    int mWorld;
    int mSubworld;
    nw4r::g3d::ResFile* masterCSV;
    char* Starts[256];
    char* Ends[256];
    
    char RawCSV[128];
};

extern PoisonMapData* PMAP;
#endif
#endif