#include "PADV-NEXT/ExecMng.h"
const char* Signature = "\nIn some cases, extreme high blood sugar can lead to coma or death.\nOther risks include decreases in white blood cells, which can be serious,\ndizziness upon standing, seizures, trouble swallowing,\nand impaired judgement or motor skill\n";
int FrameTimer = 0;
ExecPhase currentPhase;
bool isInitialized = false;
bool isInStage = false;
bool EnteredStage = false;
dEn_c* smitePlayer = NULL;

dAcPy_c* Players[4] = {NULL};
dGameKeyCore_c* Inputs[4] = {NULL};
OverlayObjectBase* OverlayObjects[MAX_OVERLAYOBJECTS] = {NULL};
const Modifier Mods[MODS_SIZE] = {
    //W1
    Modifier(41, 1, 0, POST, (void (*)(void *))NahFuckThat, 0, (void *)true, true, true),
    Modifier(1, 1, 0, POST, (void (*)(void *))Lonely, TIMER_CLEAR, (void *)NULL, false, false),
    Modifier(2, 1, 0, ALL, (void (*)(void *))SpinEternally, 0, (void *)NULL, false, false),
    Modifier(3, 1, 0, PRE, (void (*)(void *))MiniPlusPlus, 0, (void *)NULL, false, false),
    Modifier(22, 1, 0, POST, (void (*)(void *))TowerFunc, 0, (void *)NULL, false, true),
    Modifier(4, 1, 0, POST, (void (*)(void *))MarioCantBreatheUnderwater, 0, (void *)NULL, false, false),
    Modifier(5, 1, 0, PRE, (void (*)(void *))SuperCold, 0, (void *)NULL, false, false),
    Modifier(6, 1, 0, POST, (void (*)(void *))ShyRollers, 30, (void *)NULL, false, false),
    Modifier(24, 1, 0, POST, (void (*)(void *))TrustYourSenses, 0, (void *)NULL, false, true),
    //W2
    Modifier(1, 2, 0, POST, (void (*)(void *))NoTakeBacks, 0, (void *)NULL, false, false),
    Modifier(2, 2, 0, POST, (void (*)(void *))Inverter, 0, (void *)NULL, false, false),
    Modifier(3, 2, 0, PRE, (void (*)(void *))LiterallyBulletHell, 0, (void *)NULL, false, false),
    Modifier(22, 2, 0, PRE, (void (*)(void *))WeGoWee, 0, (void *)NULL, false, false), 
    Modifier(4, 2, 0, POST, (void (*)(void *))SandyPain, 0, (void *)NULL, false, false),
    Modifier(5, 2, 0, POST, (void (*)(void *))PokeyLanParty, 6, (void *)NULL, false, false),
    Modifier(6, 2, 0, PRE, (void (*)(void *))FuckTwoSix, 0, (void *)NULL, false, true),
    Modifier(24, 2, 0, PRE, (void (*)(void *))FixRoys, 0, (void *)NULL, false, false), //Fixes Roy's endless loop by avoiding L:24,W:2,A:0
    Modifier(24, 2, 0, POST, (void (*)(void *))CastleBlowers, 6, (void *)(bool)dInfo_c::m_instance->m_startGameInfo.mArea, true, false), //Actual OverlayObject implementation, uses L:24 as a fake escape from Roy's A:0, also to seperate exec
    //W3
    Modifier(1, 3, 0, PRE, (void (*)(void *))MarioSlide, 0, (void *)NULL, false, false),
    Modifier(2, 3, 0, PRE, (void (*)(void *))RealisticBullet, 0, (void *)NULL, false, true),
    Modifier(3, 3, 0, PRE, (void (*)(void *))Icey, 0, (void *)NULL, false, true),
    Modifier(21, 3, 0, POST, (void (*)(void *))BetterGhosts, 0, (void *)NULL, false, true),
    Modifier(22, 3, 0, PRE, (void (*)(void *))FloatyTower, 15, (void *)NULL, false, false),
    Modifier(4, 3, 0, PRE, (void (*)(void *))SlideyBlocks, 0, (void *)NULL, false, false),
    Modifier(5, 3, 0, PRE, (void (*)(void *))WierdLineBlock, 15, (void *)NULL, false, false),
    Modifier(24, 3, 0, PRE, (void (*)(void *))SnakeBlockFuckery, 0, (void *)NULL, false, true),
    //W4
    Modifier(1,4,0, PRE, (void (*)(void *))WaterStrangeness, 15, (void*)NULL, false, false),
    Modifier(2,4,0, PRE, (void (*)(void *))BomberFish, 60, (void*)NULL, false, false),
    Modifier(3,4,0, PRE, (void (*)(void *))HuckMeSomeBalls, 15, (void*)NULL, false, false),
    Modifier(22,4,0, ALL, (void (*)(void *))ToDefineTheWordConvey, 0, (void*)NULL, false, false)
};

PhysicsOverride* PhysicsOverrides[4] = {NULL};

/*const sBcPointData DefaultCollision[20] {
    //Normal/Walk/Jump
    CreateSensorData(0x000c8001, 0xffffb000, 0x00004000, 0x00000000), //0x0/**0
    CreateSensorData(0x005c0001, 0xffffe000, 0x00001000, 0x00010000), //0x10
    CreateSensorData(0x000c0001, 0x00004000, 0x0000b000, 0x00008000), //0x20
    CreateSensorData(0x000c0001, 0x00008000, 0x00001000, 0x00002000), //0x30

    //Crouch/Groundpound
    CreateSensorData(0x000c8001, 0xffffb000, 0x00004000, 0x00000000), //0x40
    CreateSensorData(0x005c0001, 0xffffe000, 0x00001000, 0x0000c000), //0x50
    CreateSensorData(0x000c0001, 0x00004000, 0x00008800, 0x00008000), //0x60
    CreateSensorData(0x000c0001, 0x00008000, 0x00001000, 0x00002000), //0x70

    //Unk1
    CreateSensorData(0x000c8001, 0xffffb000, 0x00004000, 0x00000000), //0x80
    CreateSensorData(0x005c0001, 0xffffe000, 0x00001000, 0x00001000), //0x90
    CreateSensorData(0x000c0001, 0xffffc000, 0xffff2000, 0x00008000), //0xA0
    CreateSensorData(0x000c0001, 0x00008000, 0x00001000, 0x00002000), //0xB0

    //Unk2
    CreateSensorData(0x000c8001, 0xffffb000, 0x00004000, 0x00000000), //0xC0
    CreateSensorData(0x005c0001, 0xffffe000, 0x00001000, 0x00010000), //0xD0
    CreateSensorData(0x000c0001, 0x00004000, 0x0000a000, 0x00008000), //0xE0
    CreateSensorData(0x000c0001, 0x00008000, 0x00001000, 0x00002000), //0xF0

    //Star::Jump
    CreateSensorData(0x000c8001, 0xffffb000, 0x00004000, 0x00000000), //0x100
    CreateSensorData(0x005c0001, 0xffffe000, 0x00001000, 0x0000c000), //0x110
    CreateSensorData(0x000c0001, 0x00004000, 0x0000a000, 0x00008000), //0x120
    CreateSensorData(0x000c0001, 0x00008000, 0x00001000, 0x00002000)  //0x130
};*/

void ExecMng::Initialize() {
    Spawners::daIcicle_c::SetGlobalScale(1); //Run this to cache SMC's to avoid false base-data
    isInitialized = true;
    OSReport("PADV: %s\n", Signature);
    OSReport("PADV Initialization Completed\n");
}

//False = onNextScene
//True = onSceneCreate
void ExecMng::Reset(bool full) {
    //OSReport("Reset Type: %i\n", full);
    dSys_c::setFrameRate(1);
    ModifyMovement(0);

    for(int i = 0; i < 4; i++) {
        if(PhysicsOverrides[i]) delete PhysicsOverrides[i];
        //if(CollisionOverrides[i]) delete CollisionOverrides[i];
    }
    
    if(!full) {
        dBaseActor_c* wmSwitchBroken = GetNextOfType(WM_SWITCH, NULL);
        if(wmSwitchBroken) {wmSwitchBroken->deleteRequest(); OSReport("Killed the Switch, hehe Killswitch\n");}
    }

    for(int i = 0; i < MAX_OVERLAYOBJECTS; i++)
            { if(OverlayObjects[i]) OverlayObjects[i]->mDelayedDelete = true; }
    if(full) {
        LivePatch(0x38600086, &LP_PARABOMBSPAWNID);
        *daBoo_c::getGlobalScaler() = 1.f;
        Spawners::daIcicle_c::SetGlobalScale(1);
        daHuckit_c::SetSpawnID(0x11F); //Huckit Rock
        EnteredStage = false;
    }

    dMj2dGame_c* header = dSaveMng_c::m_instance->getSaveGame(dSaveMng_c::m_instance->getLastSelectedFile());
    if(header) {
        header->setStockItem(0, 99);
        header->setStockItem(1, 99);
        header->setStockItem(2, 99);
        header->setStockItem(3, 99);
        header->setStockItem(4, 99);
        header->setStockItem(5, 99);
        header->setStockItem(6, 99);

        daPyMng_c::mRest[0] = LIVES_AMOUNT;
        daPyMng_c::mRest[1] = LIVES_AMOUNT;
        daPyMng_c::mRest[2] = LIVES_AMOUNT;
        daPyMng_c::mRest[3] = LIVES_AMOUNT;
    }
    #ifdef PADV_DEBUG_EXEC
    OSReport("Passed on phase %i\n", full);
    #endif
    return;
}

//BeginFrame, EndFrame
void ExecMng::Execute(ExecPhase phase) {
    if(!isInitialized) return;

    if (!GetPlayers() || !GetAllInputs()) return;
    isInStage = (bool)(dScStage_c::m_instance && Players[0] && Players[0]->mPos.x != 0);
    HandleHotkeys();
    currentPhase = phase;
    if (phase == PRE)
    {
        FrameTimer++;
        #ifdef DEBUG_EXEC_EXTREME
            OSReport("BeginFrame\n");
        #endif
        if(smitePlayer) {
            smitePlayer->mPos.y += -2048;
            smitePlayer = NULL;
        }
        AntiBubble();
    }
    else if(phase == POST) {
        if(FrameTimer > 60) FrameTimer = 0;
        #ifdef DEBUG_EXEC_EXTREME
            OSReport("EndFrame\n");
        #endif
    }

    if(!isInStage) {
        #ifdef PADV_MAPCORE
        PoisonMap* map = PMAP;
        
        if(map) {
            map->Execute(phase);
        }
        #endif
        return;
    }

    for(int i = 0; i < MODS_SIZE; i++) {
        #ifdef DEBUG_EXEC_EXTREME
        OSReport("Running Mods %i\n", i);
        #endif
        Mods[i].TryRun(phase);
    }
    for(int i = 0; i < MAX_OVERLAYOBJECTS; i++) {
        if(OverlayObjects[i] && OverlayObjects[i]->Verify()) OverlayObjects[i]->Execute();
    }

#ifdef DEBUG_EXEC_EXTREME
    OSReport("Frame: %i\n", FrameTimer);
    #ifdef DEBUG_PLAYERCOUNT
    OSReport("Players: %i, %i, %i, %i\n", (bool)Players[0], (bool)Players[1], (bool)Players[2], (bool)Players[3]);
#endif
#endif
    return;
}

void ExecMng::OnPhysicsUpdate(dAcPy_c* plr) {
    asm("mr r3, r31");
    for(int i = 0; i < 4; i++) {
        if(PhysicsOverrides[i]) {
            PhysicsOverrides[i]->Apply();
        }
    }
    return;
}

//Im proud of this. Well, rather the total mechanism.
void* ExecMng::OnBcPickSource(void* out) {
    register void* ptr;
    register void* offset;
    register u32 type;
    asm("mr type, r4");
    type /= 4;
    asm("mr ptr, r28");
    asm("mr offset, r5");
    const TileCollisionOverride* chosen = NULL;
    for(int i = 0; i < 4; i++) {
        if(PhysicsOverrides[i] && PhysicsOverrides[i]->mOwner == ptr && PhysicsOverrides[i]->State[3])
        {
            if(type == 0x00000000) chosen = &PhysicsOverrides[i]->NormalCollision;
            else if(type != 0x3 && type > 0) chosen = &PhysicsOverrides[i]->SuperCollision;
            else if(type == 0x00000003) chosen = &PhysicsOverrides[i]->MiniCollision;
        }
    }
    if(chosen) out = GetMemberFromOffset((void*)chosen, (u32)offset * 0x40);
    return out;
}

ext void* LookupBc() {
    asm("lis r3,0x8032");
    asm("rlwinm r4,r4,2,0,29");
    asm("addi r3,r3,21972");
    asm("rlwinm r0,r5,6,0,25");
    asm("lwzx r3,r3,r4");
    asm("add r3,r3,r0");
    asm("blr");
    //Replaces the original function
}

//Execution Hooks
kmBranchDefCpp(0x800e4814, NULL, void, void) { ExecMng::Execute(PRE); }
kmBranchDefCpp(0x800e4874, NULL, void, void) { ExecMng::Execute(POST);}
kmBranchDefCpp(0x800552b0, NULL, void, void) { ExecMng::OnPhysicsUpdate(NULL); }
kmCallDefCpp(0x80142784, void, void)         { ExecMng::OnBcPickSource(LookupBc()); }

kmBranchDefCpp(0x8015d82c, NULL, void, void) { ExecMng::Initialize(); }
kmBranchDefCpp(0x800bbbb4, NULL, void, void) { ExecMng::Reset(true);  }
kmBranchDefCpp(0x800e2030, NULL, void, void) { ExecMng::Reset(false); }

#ifdef PADV_PMAP_ENABLE
kmBranchDefCpp(0x800F36B0, NULL, void, int w, int sw) { PoisonMapData::Initialize(w, sw); }
#endif

//Static Patch section
#ifdef DEBUG_PADV
kmWrite32(0x800F1960, INSTR_BLR); // Allow Debug (BLRs DeleteDebugMaterial)
#endif

kmWrite32(0x800e3ab8, 0x3c0001f4); // FuckTimers! | (Functionally) NO-OPs Timer Execution
kmWrite32(0x801591f0, 0x4e800020); // Disable1UpEffect! | Disables the 1Up Effect
kmWrite32(0x8010CDE0,  INSTR_BLR); // No Scores | BLRs ScoreManager Execution
kmWrite32(0x800b1910,  INSTR_BLR); // No Fukidashi | BLRs the Fuki Manager, to remove the pesky popups
kmWrite32(0x808efa70,  INSTR_BLR); // WMSWITCH is breaking the game, so this is a sort of tape-on fix, so i can keep developing. Something about res alloc. Idk and idc
kmWrite32(0x80907364,  INSTR_NOP); // Part 2 of ^^^ | Stops switch activation crash

//Deaht Mush Handling
kmWrite32(0x80a285f0,  INSTR_NOP); // Collect Items either way, for DeathMush Implementation
kmWrite32(0x80a285f4,  INSTR_NOP);
kmWrite32(0x80a285d4, 0x48000010);

#ifdef PADV_USE_MKWCAT_PATCHES
// No Death Pausing
kmWrite32(0x801410C4,  INSTR_NOP);
kmWrite32(0x801410D0,  INSTR_NOP);
kmWrite32(0x80141020,  INSTR_BLR);
kmWrite32(0x8013DA30,  INSTR_BLR);
kmWrite32(0x8013DB30,  INSTR_BLR);
kmWrite32(0x80150E54, 0x38600000); // li r3, 0
kmWrite32(0x80150E98, 0x38600000); // li r3, 0

// Exit Anywhere
kmWrite32(0x800B4EA8, 0x38600001); // li r3, 1

//Infinite Projectiles
kmWrite32(0x8011B0A4, 0x38600001);
kmWrite32(0x80124744, 0x38600001);
#else
kmWrite32(0x8004E050,  INSTR_BLR); // No Death Pause, pussy version
#endif
