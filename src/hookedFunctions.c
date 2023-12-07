#include "common.h"
#include "../include/mod_main.h"

void Porocess_Mode0_Hook(void) {
    u32 temp_s0;
    s32 sp28;
    s32 sp24;
    s32 i;

    switch (gGameModeState) {
    case 0:
        D_800FFDF0 = 3;
        DMAStruct_Print();
        D_80174878 += 1;
        
        if (D_800F06EC >= 0) {
            D_80174878 = D_800F06EC;
        }
        
        D_80174878 = loadStageByIndex(D_80174878);
        if (gCurrentStage == 7) {
            D_80168DA0 = (u32) gControllerNo;
            D_801749AC = 2;
        } else {
            D_801749AC = 0;
            D_80168DA0 = 1;
        }

        //required 1 liner to match
        for (i = 0; i < D_80168DA0; i++) {gPlayerActors[i].active = 1;}

        for (; i < 4; i++) {
            gPlayerActors[i].active = 0;
        }            
        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
    
        func_8002E0CC();
        InitField();
        func_80056EB4();
        aa1_InitHead();
        func_8005C9B8();
        func_80084788();
        
        D_80174980 = 0;
        if (gCurrentStage == 7) {
            func_800546F0();
        } else {
            func_8008FE00();
        }
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        TaskInit();
        if ((gCurrentStage == 0xF) || (gCurrentStage == 8)) {
            func_800C1458(0);
        }
        func_8008BE14();
        func_8008800C(8);
        gGameModeState += 1;
        func_8008F114();
        gCurrentStageTime = 0;
        return;
    case 1:
        func_8002CE54();
        return;
    case 2:
        gGameModeState = 1;
        return;
    case 3:
        D_801749AC = 0;
        temp_s0 = gPlayerActors->hp;
        sp28 = currentStageCrowns;
        sp24 = D_80247904;
        DMAStruct_Print();
        D_80174878 += 1;
        if (D_800F06EC >= 0) {
            D_80174878 = D_800F06EC;
        }
        D_80174878 = loadStageByIndex(D_80174878);
        func_8002E0CC();
        InitField();
        gPlayerActors->hp = temp_s0;
        func_80056EB4();
        aa1_InitHead();
        func_8005C9B8();
        func_80084788();
        TaskInit();
        if (D_800FFEBC != 0) {
            func_800C1458(1);
        } else {
            func_800C1458(0);
        }
        gGameModeState = 1;
        func_8008F114();
        currentStageCrowns = sp28;
        D_80247904 = sp24;
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
            return;
        }
    default:
        return;
    case 4:
        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
        
        gPlayerActors[0].active = 1;
        for (i = 1; i < 4; i++) {
            gPlayerActors[i].active = 0;
        }
        
        gNoHit = 0;
        gOneRun = 0;
        D_80200B38 = 0;
        D_801749AC = 0;
        SaveData_ReadFile(&gGameState);
        D_80174878 = gCurrentStage - 1;
        func_8008FD68();
        SaveData_ReadFile(&gGameState);
        if (D_80236974 == 1) {
            if (gCurrentStage == 0) {
                D_80236978 = 1;
            }
            func_800C2820(gGameState.gCurrentZone, &gPlayerActors[0], &gGameState);
        } else {
            D_80236978 = 0;
            func_800C1510(gGameState.gCurrentZone, gGameState.unk33);
            func_800B4574(&gGameState.unk2, &gGameState.unk_22);
            func_800C0760(gGameState.gCurrentZone);
        }
        currentStageCrowns = (s32) gGameState.stageCrowns;
        DummiedPrintf(D_8010DB1C);
        func_8008FEA8(gCurrentStage, gGameState.gCurrentZone);
        gGameModeState = 1;
        func_8008F114();
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        func_8008800C(8);
        return;
    case 5:
        setProcessType(1);
        func_8008F114();
        return;
    case 6:
        setProcessType(6);
        return;
    case 7:
        gNoHit = 0;
        gOneRun = 0;
        D_80200B38 = 0;
        D_80168DA0 = 1;
        D_801749AC = 0;
        SaveData_ReadFile(&gSaveFile);
        D_80174878 = gCurrentStage - 1;
        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
        func_8008FD68();
        SaveData_ReadFile(&gSaveFile);
        if (D_80236974 == 1) {
            if (gCurrentStage == 0) {
                D_80236978 = 1;
            }
            func_800C2820(gSaveFile.gCurrentZone, &gPlayerActors[0], &gSaveFile);
        } else {
            D_80236978 = 0;
            func_800C1510(gSaveFile.gCurrentZone, gSaveFile.unk33);
            func_800B4574(&gSaveFile.flags, &gSaveFile.unk_22);
            func_800C0760(gSaveFile.gCurrentZone);
        }
        func_8008FEA8(gCurrentStage, gSaveFile.gCurrentZone);
        currentStageCrowns = (s32) gSaveFile.stageCrowns;
        gGameModeState = 1;
        func_8008F114();
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        break;
    }
}