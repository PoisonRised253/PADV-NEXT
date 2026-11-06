#include "PADV-NEXT/PoisonMap/PoisonMap.h"
#ifdef PADV_PMAP_ENABLE
PoisonMapData *PMAP = NULL;

PoisonMapData *PoisonMapData::Initialize(int world, int subworld)
{
    PoisonMapData *out = new PoisonMapData();
    out->masterCSV = &GetMasterCSV(world);
    if (out->masterCSV == NULL)
    {
        OSReport("Failed to locate Archive. This means an Execution. Goodbye, hope you had a pleasent life :/\n\n\nAppearently this ended up working out perfectly, to where CSV data not existing just not building paths in the first place. Interesting\n");
        delete out;
        return;
    }

    out->mWorld = world;
    out->mSubworld = subworld;
    PMAP = out;

    out->CollectPoints(out->masterCSV);
    out->BuildPoints();
    out->BuildRoutes();
    out->ConvertToVanillaPathSystem();

    dResMng_c::m_instance->getResP()->deleteRes("PMAP");

    return out;
}

nw4r::g3d::ResFile PoisonMapData::GetMasterCSV(int world)
{
    char *filename;
    switch (world)
    {
    case 1:
        filename = "W1.csv";
        break;
    case 2:
        filename = "W2.csv";
        break;
    case 3:
        filename = "W3.csv";
        break;
    case 4:
        filename = "W4.csv";
        break;
    case 5:
        filename = "W5.csv";
        break;
    case 6:
        filename = "W6.csv";
        break;
    case 7:
        filename = "W7.csv";
        break;
    case 8:
        filename = "W8.csv";
        break;
    case 9:
        filename = "W9.csv";
        break;
    default:
        filename = "W1.csv";
        break;
    }

    dResMng_c *res = dResMng_c::m_instance;
    if (!res->setRes("WorldMap", "PMAP", NULL))
        return; // Shut up compiler.
    do
    {
        asm("nop");
    } while (res->syncAllRes());

    nw4r::g3d::ResFile file = res->getRes("PMAP", filename);
    if (!file.IsValid())
    {
        OSReport("File Invalid, Dropping the nukes\n");
        OSPanic("structs.hpp", 321, "File Invalid, Dropping the nukes\n");
    }
    OSReport("Obtained File Successfully\n");
    if (file.ptr())
        OSReport("MasterCSV created\n");
    return file;
}

void PoisonMapData::CollectPoints(nw4r::g3d::ResFile *csv)
{
    register u32 lr;
    asm("mflr %0" : "=r"(lr));
    const char *base = (const char *)csv->ptr()->dict.topLevel.data - 0x20;
    OSReport("LR_SAVE: %p\nString: %p\n", lr, &base);

    char *computed = NULL;
    int offset = 0;
    int outIdx = 0;

    char *start = (char *)base;
    char *end = NULL;

    do
    {
        computed = ((char *)base + offset);
        if (*computed == ';')
        {
            end = computed;
            OSReport("Creating Point | Start: %p, End: %p\n", start, end);
            Starts[outIdx] = start;
            Ends[outIdx] = end;

            outIdx++;
            offset++;
            start = ((char *)base + (offset + 1));
        }
        else if (*computed == '#')
        {
            break;
        }
        offset++;

    } while (true);
    return;
}

void PoisonMapData::BuildPoints()
{
    for (int i = 0; i < 256; i++)
    {
        if (Starts[i] && Ends[i])
        {
            OSReport("Create[%i]\n", i);
            mPoints[i] = PoisonMapPoint::Create(Starts[i], Ends[i]);
        }
        else
            break;
    }
    OSReport("End Of Point Record\n");
    return;
}

void PoisonMapData::BuildRoutes()
{
}

void PoisonMapData::ConvertToVanillaPathSystem()
{
}

PoisonMapData::PoisonMapData() {}
PoisonMapData::~PoisonMapData() { for(int i = 0; i < 256; i++) {if(mPoints[i]) mPoints[i]->~PoisonMapPoint(); mPoints[i] = NULL; }; PMAP = NULL;}

PoisonMapPoint *PoisonMapPoint::Create(const char *csvStart, const char *csvEnd)
{
    PoisonMapPoint *newPoint = new PoisonMapPoint();
    int idx = 0;
    char *next = NULL;
    do
    {
        next = ((char *)csvStart + idx);
        newPoint->RawCSV[idx] = *next;
        idx++;
    } while (next <= csvEnd);
    OSReport("RawCSV of [%i]:\n%s\n", newPoint->mID, newPoint->RawCSV);
    newPoint->ParseRawCSV();

    if(PMAP && PMAP->mPoints[newPoint->mID] == NULL) { {PMAP->mPoints[newPoint->mID] = newPoint; OSReport("Registered Sucessfully\n");} }

    return newPoint;
}

// This is currently a shitshow. FIX IT.
void PoisonMapPoint::ParseRawCSV()
{
    if (!RawCSV[0])
        return;

    mPos = mVecInt_c(0, 0, 0);
    mRot = mAng3_c(0, 0, 0);
    mScale = mVecInt_c(0, 0, 0);

    const char *basePtr = RawCSV;
    char *next = NULL;

    int item = 0;
    int offset = 0;

    char processBuffer[32];

    do
    {
        next = (char *)basePtr + offset;
        processBuffer[offset] = *next;
        if (processBuffer[offset] == ',' || processBuffer[offset] == ';' || processBuffer[offset] == '#')
        {
            bool Invalid = false;
            if(processBuffer[clamp(offset - 1, 0, 31)] == 'N') Invalid = true;
            processBuffer[offset] = '\0';
            if (item == 0)
            {
                mID = atoi(processBuffer);
            }
            else if (item == 1)
            {
                mPos.x = atoi(processBuffer);
            }
            else if (item == 2)
            {
                mPos.y = atoi(processBuffer);
            }
            else if (item == 3)
            {
                mPos.z = atoi(processBuffer);
            }
            else if (item == 4)
            {
                mRot.x = atoi(processBuffer);
            }
            else if (item == 5)
            {
                mRot.y = atoi(processBuffer);
            }
            else if (item == 6)
            {
                mRot.z = atoi(processBuffer);
            }
            else if (item == 7)
            {
                mScale.x = atoi(processBuffer);
            }
            else if (item == 8)
            {
                mScale.y = atoi(processBuffer);
            }
            else if (item == 9)
            {
                mScale.z = atoi(processBuffer);
            }
            else if (item == 10)
            {
                if(!Invalid)mNeighbors[0] = atoi(processBuffer);
                else mNeighbors[0] = 0xFF;                
            }
            else if (item == 11)
            {
                if(!Invalid)mNeighbors[1] = atoi(processBuffer);
                else mNeighbors[1] = 0xFF;
            }
            else if (item == 12)
            {
                if(!Invalid)mNeighbors[2] = atoi(processBuffer);
                else mNeighbors[2] = 0xFF;
            }
            else if (item == 13)
            {
                if(!Invalid)mNeighbors[3] = atoi(processBuffer);
                else mNeighbors[3] = 0xFF;
            }
            else if (item == 14)
            {
                mNodeType = atoi(processBuffer);
            }
            else if (item == 15)
            {
                mLevel = atoi(processBuffer);
            }
            else if (item == 16)
            {
                mWorld = atoi(processBuffer);
            }
            for(int i = 0; i < 32; i++) processBuffer[i] = '\n';
            item++;
        }
        offset++;
    } while (item < 17);
    OSReport(
        "Point Created!\n---mID: %i---\n---X: %i---\n---Y: %i---\n---Z: %i---\n---RX: %i---\n---RY: %i---\n---RZ: %i---\n---SX: %i---\n---SY: %i---\n---SZ: %i---\n---Neighbors[]: {%i,%i,%i,%i}---\n---NodeType: %i---\n---Level: %i---\n---World: %i---\n",
        mID,
        mPos.x,
        mPos.y,
        mPos.z,
        mRot.x.abs(),
        mRot.y.abs(),
        mRot.z.abs(),
        mScale.x,
        mScale.y,
        mScale.z,
        mNeighbors[0],
        mNeighbors[1],
        mNeighbors[2],
        mNeighbors[3],
        mNodeType,
        mLevel,
        mWorld
    );
}

PoisonMapPoint::PoisonMapPoint() {};
PoisonMapPoint::~PoisonMapPoint() {};

#endif
