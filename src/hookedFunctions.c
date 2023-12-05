#include "common.h"
#include "../include/mod_main.h"
#include "../include/menu.h"

s32 freezeTimer = 0;
s32 timerParametersBool = 0;

#define CAKE_INIT 0
#define CAKE_CHOCO_KIDS_ENTER 1
#define CAKE_CHOCO_KIDS_ENTER_PHASE_2 2

#define CAKE_SPIN 4
#define CAKE_MOVE_AND_LAUNCH_BERRIES 5
#define CAKE_SHOW_CHOCO_KIDS 6
#define CAKE_DEATH 7

#define CAKE_STATE actor->userVariables[0]
#define CAKE_MOVE_TIMER actor->userVariables[1]
#define CHOCO_KID_COUNT actor->userVariables[2]

#define PI 3.141592653589793

s32 secondPhase = 0;
s32 chocokidSpawnTimer = 0;
s32 cakePhase3Hits = 0;
s32 hasEnteredPhase3 = 0;
s32 soundEffect =  0;

void ActorInit_CakeBoss_Hook(Actor* actor) {
    actor->tongueCollision = 3;
    CHOCO_KID_COUNT = 8; 
    secondPhase = 0;
    actor->unknownPositionThings[1].unk_0C = actor->tScale;
    actor->unknownPositionThings[1].unk_04 = 250;
    actor->unknownPositionThings[1].unk_10 = actor->tYPos;
    actor->unknownPositionThings[1].unk_08 = 0.0f;
    actor->unknownPositionThings[1].unk_00 = 0.0f;
    actor->unknownPositionThings[2].unk_08 = 0.0f;
    actor->unknownPositionThings[2].unk_04 = 0.0f;
    actor->unknownPositionThings[2].unk_00 = 0.0f;
    actor->unknownPositionThings[2].unk_0C = 200;
    actor->unknownPositionThings[2].unk_10 = actor->tYPos * 2;
    chocokidSpawnTimer = 90; //spawn choco kid every 3 seconds
    cakePhase3Hits = 0;
    hasEnteredPhase3 = 0;
    soundEffect = 0;
}

void incrementCakeHitCounter(void) {
    if (hasEnteredPhase3 == 1) {
        cakePhase3Hits++;
        playSoundEffect(0xB9, 0, 0, 0, 0, 0x0010); //books phase 2 getting hit by cue ball noise
    }
}


void ActorTick_CakeBoss_Hook(Actor* actor) {
    f32 temp_f12;
    f32 temp_f20;
    f32 xDistFromDestination;
    f32 zDistFromDestination;
    f32 temp_f26;
    f32 temp_f30;
    f32 temp_f6;
    f32 var_f30;
    u32 var;
    s32 i;

    temp_f20 = CalcAngleBetween2DPoints(actor->pos.x, actor->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z);
    if (CHOCO_KID_COUNT == 4) {
        if (secondPhase == 0) {
            secondPhase = 1;
            actor->unk_F0 = 0x0E;
            actor->globalTimer = 0x77;
            CAKE_STATE = CAKE_INIT;
            actor->vel.z = 0.0f;
            actor->vel.x = 0.0f;
            actor->unk_15C = 35.0f; //double cake speed when running from player before opening
            actor->unk_128 = 65; //how long cake is vulnerable
        }
    } else if (CHOCO_KID_COUNT == 0) {
        if (hasEnteredPhase3 == 0) {
            hasEnteredPhase3 = 1;
            CAKE_STATE = CAKE_SPIN;
            CAKE_MOVE_TIMER = 0;
            for (i = 0; i < 6; i++) {
                Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, ((f32) i * 60.0f) + 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, i, actor->unk_12C, 0);
            }
            Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 6, actor->unk_12C, 0);
        }
    }

    if (hasEnteredPhase3  == 1) {
        if ((actor->unk_F0 -1) != 0) {
            actor->unk_F0--;
        }
    }

    if (hasEnteredPhase3 == 1 && cakePhase3Hits == 5) {
        CAKE_STATE = CAKE_DEATH;
        func_80087358(actor->userVariables[3]); //kill sounds
        actor->vel.z = 0.0f;
        actor->vel.x = 0.0f;       
    }
    if (secondPhase == 1) {
        if (actor->globalTimer == 0xD1) {
            actor->globalTimer = 0x128;
            for (i = 0; i < 6; i++) {
                Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, ((f32) i * 60.0f) + 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, i, actor->unk_12C, 0);
            }
            Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 6, actor->unk_12C, 0);
        }
    }

    if (CAKE_STATE == CAKE_SPIN || CAKE_STATE == CAKE_MOVE_AND_LAUNCH_BERRIES) {
        if (secondPhase == 1) {
            if (gActorCount < 50) {
                if (--chocokidSpawnTimer == 0) {
                    Actor_Init(Choco_Kid, actor->pos.x - 300.0f, actor->pos.y, actor->pos.z, 0 + 180.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0);   
                    chocokidSpawnTimer = 90;
                }
            }
        }
    }

    switch (CAKE_STATE) {
    case CAKE_INIT:
        if (actor->globalTimer >= 0x78U) {
            if (actor->globalTimer == 0xC) {
                actor->userVariables[3] = playSoundEffect(0x9A, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
            }
            actor->unk_F0++;
            if (actor->unk_F0 == 0xF) {
                actor->unk_F0 = 0xE;
                CAKE_STATE = CAKE_CHOCO_KIDS_ENTER;
                for (i = 0; i < 4; i++) {
                    temp_f26 = (f32) i * 90.0f;
                    temp_f12 = (temp_f26 * 2) * PI / 360.0;
                    Actor_Init(Cake_Boss_Choco_Kid, (__cosf(temp_f12) * 800.0f) + actor->pos.x, actor->pos.y + 1500.0f, (-__sinf(temp_f12) * 800.0f) + actor->pos.z, temp_f26 + 180.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, temp_f26, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 0, 0, 0);                    
                }
            }
        }
        break;
    case CAKE_CHOCO_KIDS_ENTER:
        var = 0x136 - actor->globalTimer;
        var_f30 = var;
        if (var_f30 == 15.0f) {
            for (i = 0; i < 6; i++) {
                Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, ((f32) i * 60.0f) + 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, i, actor->unk_12C, 0);
            }
            Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 6, actor->unk_12C, 0);
            playSoundEffect(0x9D, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
        }
        if ((actor->globalTimer >= 0xE2U) && ((u32) (actor->globalTimer % 15U) < 6U)) {
            actor->unk_134[3] += actor->unk_164;
            WrapDegrees(&actor->unk_134[3]);
        }
        if (var_f30 < 15.0f) {
            actor->unk_F0 = (u32) var_f30;
            func_800410B4(actor);
            if (var_f30 == 0.0f) {
                CAKE_STATE = CAKE_SPIN;
                actor->userVariables[3] = playSoundEffect(0x9A, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
                actor->unk_134[4] = (f32) (((Random(0, 0x270F) & 1) * 2) - 1) * actor->position._f32.y;
            }
        }
        break;
    case CAKE_SPIN: //4
        CAKE_MOVE_TIMER++;
        if (actor->unk_124 == CAKE_MOVE_TIMER) {
            temp_f6 = CalcAngleBetween2DPoints(actor->pos.x, actor->pos.z, actor->unk_168, actor->unk_16C);
            temp_f6 = temp_f6 * 2;
            CAKE_STATE = CAKE_MOVE_AND_LAUNCH_BERRIES;
            CAKE_MOVE_TIMER = 0;
            actor->vel.z = 0;
            actor->vel.x = 0;
            //decide cake X and Z position before opening up
            //X
            actor->unk_134[0] = (__cosf(((temp_f6 * PI) / 360.0)) * (actor->unk_170 * 0.6f)) + actor->unk_168;
            //Z
            actor->unk_134[1] = (-__sinf((((temp_f6 * 2) * PI) / 360.0)) * (actor->unk_170 * 0.6f)) + actor->unk_16C;
            //yaw
            actor->unk_134[2] = CalcAngleBetween2DPoints(gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z, actor->pos.x, actor->pos.z);
            func_80087358(actor->userVariables[3]);
            playSoundEffect(0x9B, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
        } else {
            if (CAKE_MOVE_TIMER < (actor->unk_124 - 30)) {
                s32 var = CHOCO_KID_COUNT;
                if (var >= 4){
                    var = 4;
                }
                actor->unk_90 += actor->unk_134[4];
                WrapDegrees(&actor->unk_90);
                actor->unk_94 = ((((8 - var) * actor->position._f32.x) / 4) * (180.0f - ReflectAngleToUpperQuadrants(actor->unk_90 - temp_f20))) / 180.0f;
            } else {
                actor->unk_90 += actor->unk_134[4] * 2;
                WrapDegrees(&actor->unk_90);
                actor->unk_94 = 0;
            }
            func_800382F4(actor);
        }
        break;
    case CAKE_MOVE_AND_LAUNCH_BERRIES: //5
        xDistFromDestination = actor->pos.x - actor->unk_134[0]; //X destination
        zDistFromDestination = actor->pos.z - actor->unk_134[1]; //Z destination
        CAKE_MOVE_TIMER++;
        actor->vel.x = __cosf((((actor->unk_134[2] * 2) * PI) / 360.0)) * actor->unk_15C;
        actor->vel.z = -__sinf((((actor->unk_134[2] * 2) * PI) / 360.0)) * actor->unk_15C;
        func_8002D36C(&actor->unk_134[2], CalcAngleBetween2DPoints(actor->pos.x, actor->pos.z, actor->unk_134[0], actor->unk_134[1]), actor->unk_160);
        if ((actor->unk_12C * 8) < CAKE_MOVE_TIMER) {
            if (SQ(xDistFromDestination) + SQ(zDistFromDestination) < (actor->unk_15C * 8.0f) || (actor->unk_170 / actor->unk_15C) == (f32) CAKE_MOVE_TIMER) {
                if (hasEnteredPhase3 == 1) {
                    CAKE_STATE = CAKE_SPIN;
                    CAKE_MOVE_TIMER = 0;
                    for (i = 0; i < 6; i++) {
                        Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, ((f32) i * 60.0f) + 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, i, actor->unk_12C, 0);
                    }
                    Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 6, actor->unk_12C, 0);
                } else {
                    CAKE_STATE = CAKE_SHOW_CHOCO_KIDS;
                    CAKE_MOVE_TIMER = 0;
                    actor->vel.z = 0.0f;
                    actor->vel.x = 0.0f;
                }
            }
        }
        break;
    case CAKE_SHOW_CHOCO_KIDS:
        CAKE_MOVE_TIMER++;
        temp_f30 = (f32) (actor->unk_128 - CAKE_MOVE_TIMER);
        if (temp_f30 == 15.0f) {
            for (i = 0; i < 6; i++) {
                Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, ((f32) i * 60.0f) + 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, i, actor->unk_12C, 0);
            }
            Actor_Init(Cake_Boss_Strawberry, actor->pos.x, actor->pos.y + 450.0f, actor->pos.z, 0.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, actor->actorIndex, 6, actor->unk_12C, 0);
            if (hasEnteredPhase3 == 0) {
                playSoundEffect(0x9D, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
            }
        }
        if (temp_f30 < 15.0f) {
            actor->unk_F0 = (s32) (u32) temp_f30;
            func_800410B4(actor);
            if (temp_f30 == 0.0f) {
                CAKE_STATE = CAKE_SPIN;
                CAKE_MOVE_TIMER = 0;
                //if (hasEnteredPhase3 == 0) {
                    actor->userVariables[3] = playSoundEffect(0x9A, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
                //}
                actor->unk_134[4] = (f32) (((Random(0, 9999) & 1) * 2) - 1) * actor->position._f32.y;
            } else {
                goto block_46;
            }
        } else {
            if ((u32) actor->unk_F0 < 0xEU) {
                if (actor->unk_F0 == 0) {
                    playSoundEffect(0x9D, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
                }
                actor->unk_F0++;
                func_800410B4(actor);
            } else if ((CAKE_MOVE_TIMER % 14) == 0) {
                playSoundEffect(0x9E, &actor->pos.x, &actor->pos.y, &actor->pos.z, 0, 0);
            }
block_46:
            if (actor->globalTimer % 15U < 6U) {
                actor->unk_134[3] += actor->unk_164;
                WrapDegrees(&actor->unk_134[3]);
            }
        }
        break;
    case CAKE_DEATH:
        func_800313BC(actor->actorIndex, actor->unk_90);
        D_80174980 = 3;
        break;
    }
    func_800360E4(actor);
}

void debugMain_Hook(void) {
    volatile s32 sp64;
    unk80174880* var_s0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f2;
    f32 temp_f2_2;
    Collision* temp_v0;
    s32 i;

    if (sDebugInt >= 0) {
        if ((D_80174980 == 2) || (D_80174980 == 3) || (D_80174980 == 4) || (D_80174980 == 5)) {
            Debug_ZeroInt();
        }
        switch (gCurrentStage) {                    /* switch 1 */
        case 1:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 2:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 4:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 3:                                     /* switch 1 */
            D_800F070C = 5000.0f;
            D_800F0710 = -10000.0f;
            D_800F0714 = 0.0f;
            break;
        case 6:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 5:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 0:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 7:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 8:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        default:                                    /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        }
        
        if ((gContMain->buttons0 & 0x20) && (sDebugInt == 1)) {
            setPrimColor(0, 0, 0, 0x80);
            printUISprite(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 80.0f, 192.0f, 0.0f, 0);
            SetTextGradient(0xBF, 0xAU, 0, 0xFF, 0xC8, 0xC8, 0, 0xFF, 0xBF, 0xA, 0, 0xFF, 0xC8, 0xC8, 0, 0xFF);
            switch (gCurrentStage) {                /* switch 2 */
            case 1:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＡＮＴ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 2:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＢＯＭ", 1);
                printNumber(32.0f, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 4:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＫＩＤＳ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 3:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0f, 0.0f, 1.0f, "ＤＥＳＥＲＴ", 1);
                printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 6:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＯＰＥＮＩＮＧ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 5:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0f, 0.0f, 1.0f, "ＧＨＯＳＴ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 0:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＪＵＮＧＬＥ", 1);
                if (D_80236974 == 0) {
                    printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) (gCurrentZone + D_800F0B40), 2, 0);
                } else {
                    printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                    D_800F0B40 = D_802478E0;
                }
                break;
            case 7:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＶＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 8:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＴＲＡＮＩＮＩＧ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 9:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＪＵＮＧＬＥＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 10:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＡＮＴＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 11:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＢＯＭＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 12:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＤＥＳＥＲＴＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 13:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＫＩＤＳＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 14:                                /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＧＨＯＳＴＢＯＳＳ", 1);
                printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            }
            printNumberWR(32.0, 96.0f, 0.0f, 0.5, D_80176B74->pos.x + D_800F070C, 6, 0);
            printNumberWR(32.0, 112.0f, 0.0f, 0.5, D_80176B74->pos.y + D_800F0710, 6, 0);
            printNumberWR(32.0, 128.0f, 0.0f, 0.5, D_80176B74->pos.z + D_800F0714, 6, 0);
            temp_f0 = D_801768A0.unk34 - D_801768A0.unk28;
            temp_f2 = D_801768A0.unk38 - D_801768A0.unk2C;
            temp_f14 = D_801768A0.unk3C - D_801768A0.unk30;
            //printNumberWR(32.0, 160.0f, 0.0f, 0.5, __sqrtf(SQ(temp_f0) + SQ(temp_f2) + SQ(temp_f14)), 6, 0);
            
            // for (i = 0, sp64 = 0x20; i < 10; i++) {
            //     temp_f20 = (f32)sp64;
            //     printNumberWR(200.0f, temp_f20, 0.0f, 0.5, D_80174880->unk_00[i], 4, 1);
            //     printNumberWR(230.0f, temp_f20, 0.0f, 0.5, D_80174880->unk_28[i], 4, 1);
            //     printNumberWR(260.0f, temp_f20, 0.0f, 0.5f, D_80174880->unk_50[i], 4, 1);  
            //     sp64 += 0x0c;
            // }

            // printNumberWR(290.0f, 32.0, 0.0f, 0.5, D_80174880->unk_78, 4, 1);
            // printNumberWR(290.0f, 44.0f, 0.0f, 0.5, D_80174880->unk_7C, 4, 1);
            temp_v0 = &gZoneCollisions[gCurrentZone];
            temp_f0_2 = (temp_v0->rect_30.max.x - temp_v0->rect_30.min.x) / 2;
            temp_f16 = (temp_v0->rect_30.max.y - temp_v0->rect_30.min.y) / 2;
            temp_f20_2 = (temp_v0->rect_30.max.z - temp_v0->rect_30.min.z) / 2;
            func_80080C28(temp_v0->rect_30.min.x + temp_f0_2,
                temp_v0->rect_30.min.y + temp_f16, temp_v0->rect_30.min.z + temp_f20_2,
                temp_f0_2, temp_f16, temp_f20_2, 0x64, 0x64, 0x64, 0x64);
        }
        if (sDebugInt == 0) {
            if (func_80055E5C(U_JPAD) != 0) {
                sDebugCodeSeqStep = 1;
            }
            if ((func_80055E5C(D_JPAD) != 0) && (sDebugCodeSeqStep > 0)) {
                sDebugCodeSeqStep = 2;
            }
            if ((func_80055E5C(L_JPAD) != 0) && (sDebugCodeSeqStep >= 2)) {
                sDebugCodeSeqStep = 3;
            }
            if ((func_80055E5C(R_JPAD) != 0) && (sDebugCodeSeqStep >= 3)) {
                sDebugCodeSeqStep = 4;
                sDebugInt = 1;
            }
        } else {
            Debug_MovePlayer();
            Debug_ChangeRoom();
            Debug_NOOP();
            Debug_ChangeView();
        }
    }    
}

void Debug_ChangeRoom_Hook(void) {
    s32 shouldChangeRoom = FALSE;

    //if c-left
    if (func_80055E5C(2) != 0) {
        shouldChangeRoom = TRUE;
        gCurrentZone--;
    }
    //if c-right
    if (func_80055E5C(1) != 0) {
        shouldChangeRoom = TRUE;
        gCurrentZone++;
    }
    //if less than 0, go to current room count max - 1
    if (gCurrentZone < 0) {
        gCurrentZone = D_802478E0 - 1;
    }
    //if greater than or equal to max, set to 0
    if (gCurrentZone >= D_802478E0) {
        gCurrentZone = 0;
    }
    if (shouldChangeRoom == TRUE) {
        if (D_80236974 == 0) {
            func_800C1458(gCurrentZone);
            return;
        }
        func_800C29D8(gCurrentZone);
    }
}

void ChameleonFromDoor_Hook(playerActor* player, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Door* currentDoor;
    Door* door;
    s32 i;
    s32 pad[3];
    Vec3f sp24;
    
    currentDoor = NULL;
    door = gDoors;
    for (i = 0; i < gDoorCount; i++, door++) {
        if (arg4 == door->direction && arg2 == door->unk38 && arg3 == door->unk3C) {
            currentDoor = door;
            break;
        }
    }
    
    isChange.unk18 = i;
    func_800C08B8(&sp24, player, currentDoor);
    
    isChange.unk4C = sp24.x;
    isChange.unk50 = sp24.y;
    isChange.unk54 = sp24.z;
    isChange.unk40 = (gCardinalDirections[currentDoor->direction].unk0 * currentDoor->toX) + isChange.unk4C;
    isChange.unk44 = isChange.unk50;
    isChange.unk48 = (gCardinalDirections[currentDoor->direction].unk4 * currentDoor->toZ) + isChange.unk54;
    func_800D34CC();
}


void func_800C0CDC_Hook(playerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32* stageFrameCount = (s32*)0x801749A0;
    if (gZoneCollisions[arg1].unk7C != 0) {
        gGameModeState = 3;
        switch (gCurrentStage) {                    /* switch 1 */
        case 0:                                     /* switch 1 */
            D_80174878 = 8;
            return;
        case 1:                                     /* switch 1 */
            D_80174878 = 9;
            return;
        case 2:                                     /* switch 1 */
            D_80174878 = 0xA;
            return;
        case 3:                                     /* switch 1 */
            D_80174878 = 0xB;
            return;
        case 4:                                     /* switch 1 */
            D_80174878 = 0xC;
            return;
        case 5:                                     /* switch 1 */
            D_80174878 = 0xD;
            return;
        case 15:                                    /* switch 1 */
            switch (arg1) {                         /* switch 2 */
            case 1:                                 /* switch 2 */
                D_80174878 = 8;
                return;
            case 3:                                 /* switch 2 */
                D_80174878 = 9;
                return;
            case 2:                                 /* switch 2 */
                D_80174878 = 0xA;
                return;
            case 4:                                 /* switch 2 */
                D_80174878 = 0xB;
                return;
            case 5:                                 /* switch 2 */
                D_80174878 = 0xC;
                return;
            case 6:                                 /* switch 2 */
                D_80174878 = 0xD;
                return;
            }
            break;
        default:                                    /* switch 1 */
            gGameModeState = 2;
        }
    }
    func_800C0760(arg1);
    ChameleonFromDoor(arg0, arg1, arg2, arg3, arg4);
    func_800BFCD0();
}

void videoproc_Hook(s32 arg0) {
    s32 sp54;
    OSMesg sp58;
    s32 var_s2;

    var_s2 = 1;
    func_800A7844();
    osCreateMesgQueue(&D_801B3120, &D_801B30A0, 0x14);
    osSetEventMesg(0xEU, &D_801B3120, (void* )7);
    osSetEventMesg(9U, &D_801B3120, (void* )2);
    osSetEventMesg(4U, &D_801B3120, (void* )1);
    osViSetEvent(&D_801B3120, NULL, 2U);
    func_8008C610();

    while (1) {
        while (isSaveOrLoadActive != 0) {}
        osRecvMesg(&D_801B3120, &sp58, 1);
        if ((u32) sp58 >= 8U) {
            continue;
        }
        while (isSaveOrLoadActive != 0) {}
        switch ((u32) sp58) {
        case 0:
            sp54 = var_s2 + 1;
            osSetTimer(&D_801B3148, 0x8F184, 0, &D_801B3120, (void*) 6);
            if (sp54 != 0) {
                if (osSendMesg(&D_801192E8, NULL, 0) == -1) {
                    DummiedPrintf("Gfx送信失敗\n");
                } else {
                    var_s2 ^= 1;
                }
            } else {
                var_s2 ^= 1;
            }
            func_800A78D0();
            continue;
        case 1:
            if (D_800FF5F0 == 2) {
                DummiedPrintf("Ae");
                func_8008C6D4();
                D_800FF5F0 = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else if (D_800FF5F0 == (s16) 1) {
                DummiedPrintf("Gy");
                func_8008C464();
                D_800FF5EC = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else {
                DummiedPrintf("Ge");
                func_8008C464();
                D_800FF5EC = 0;
            }
            if (D_800FF5C8 != 0) {
                osSendMesg(&D_801B3120, (void* )3, 0);
            }
            continue;
        case 2:
            DummiedPrintf("D");
            if (D_800FF5CC != 0) {
                D_800FF5CC -= 1;
                if (D_800FF5CC == 0) {
                    osViBlack(0U);
                }
            }
            if (D_800FF5D8 == (s16) 1) {
                D_800FF5D8 = 0;
            }
            osSendMesg(&D_801192D0, (void* )2, 0);
            func_8008C4E8();
            continue;
        case 3:
            if (D_800FF5C4 != 0) {
                DummiedPrintf("Res ");
            } else if (D_800FF5F0 != 0) {
                D_800FF5C8 = 1;
                DummiedPrintf("Sw ");
            } else {
                DummiedPrintf("Gs ");
                D_800FF5C8 = 0;
                osWritebackDCacheAll();
                osSpTaskLoad(D_801B3138);
                osSpTaskStartGo(D_801B3138);
                func_8008C440();
                D_800FF5EC = (s16) 1;
                D_800FF5D8 = (s16) 1;
            }
            continue;
        case 4:
            continue;
        case 5:
            if (D_800FF5C4 == 0) {
                osRecvMesg(&D_801192B8, NULL, 0);
                DummiedPrintf("As");
                D_800FF5F0 = 2;
                osWritebackDCacheAll();
                osSpTaskLoad((OSTask* ) D_801B3140);
                osSpTaskStartGo((OSTask* ) D_801B3140);
                func_8008C698();
            }
            continue;
        case 7:
            D_800FF5C4 = 1;
            osViBlack(1);
            osViSetYScale(1.0f);
            func_8007B174();
            Rumble_StopAll();
            continue;
        }
        D_800FF5F0 = 1;
        if (osSendMesg(&D_801B35A0, NULL, 0) != -1) {
            continue;
        }
        DummiedPrintf("Audio送信失敗\n");
        continue;   
    }
}

extern s32 D_801749AC;
void AddCarrot(s32);
void func_80078294(f32, f32, s32, f32, s32);
s32 isntNegative(s32);
extern s32 currentStageCrowns;
extern s32 D_80247904;
extern s16 D_800FFEBC;
extern s16 D_80200B38;
extern s32 D_80236978;
extern char D_8010DB1C[];
//static SaveFile gSaveFile;
extern SaveFile gSaveFile; //should be static in matching code, but we dont want to declare this again
s32 SaveData_ReadFile(SaveFile*);
void func_8002CE54(void);
void func_800546F0(void);
void func_8008800C(s32);
s32 func_8008BE14(void);
void func_8008FD68(void);
void func_8008FE00(void);
void func_8008FEA8(s32, u8);
void func_800B4574(u8*, s16*);
void func_800C1458(s32);
void func_800C1510(u8, u8);
void func_800C2820(u8, playerActor*, SaveFile*);

char* ParseIntToBase10(s32, s32*);                         /* extern */
extern char D_8010D8E8[];
extern char D_8010D8F8[];
extern char D_8010D8FC[];
s32 func_80080430(f32, f32, f32, f32, f32, f32, char*, s32);
extern u32 gNextZone;
s32 saveTimeBool = 0;
//gCurrentStage, gCurrentZone, 
u64 BestRTATimes[8][30][30][2];
extern char pathTimes[];
extern FIL sdsavefileTimes;

void setFreezeTimerC(void) {
    if (timerParametersBool == 1) {
        timerParametersBool = 0;
        freezeTimer = 60; //2 seconds
        zoneExitID = gCurrentZone;
        displayTimeRTA = totalElapsedCounts;
        displayTimeIGT = gCurrentStageTime;
        saveTimeBool = 1;
    }
}

void func_800C54F8_Hook(Vec2s* arg0, s32* arg1) {
    arg0->x = 0; //clears value so IGT continues to advance
    arg0->y = 0;
    *arg1 = 0;
    freezeTimer = 0;
    prevDoorEntryTime = displayTimeRTA;
    // if (*storedCount != 0) {
    //     *prevDoorEntryCount = *storedCount;
    // }
}

// f32 calculate_and_update_fps(void) {
//     u32 newTime = osGetCount();
//     u32 oldTime = frameTimes[curFrameTimeIndex];
//     frameTimes[curFrameTimeIndex] = newTime;

//     curFrameTimeIndex++;
//     if (curFrameTimeIndex >= FRAMETIME_COUNT) {
//         curFrameTimeIndex = 0;
//     }

//     return ( (f32)FRAMETIME_COUNT * 1000000.0f) / (s32) OS_CYCLES_TO_USEC(newTime - oldTime);
// }
extern s32 gPrevZone;

void DisplayTimerWrapper(void) {
    s32 minutes;
    s32 xPos;
    s32 seconds;
    s32 unk;

    if (toggles[TOGGLE_DISPLAY_IGT] == 0) {
        if ((gameModeCurrent == 0 || gameModeCurrent == 15) && (gCurrentStage != 8)) {
            minutes = gCurrentStageTime / 1800; //(30 frames * 60 seconds)
            seconds = (gCurrentStageTime % 1800) / 30;
            func_800610A8();
            SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);
            //print ' character
            PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8E8, 1);
            xPos = 220;
            if (minutes > 99) {
                minutes = 99;
                seconds = 59;
            }
            if (minutes < 10) {
                PrintText(0xDC, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8F8, 1);
                xPos = 228;
            }
            //var_v1_2 = ParseIntToBase10(minutes, &unk);
            PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(minutes, &unk), 1);
            xPos = 244;
            if (seconds < 10) {
                PrintText((f32) 0xF4, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8FC, 1);
                xPos = 252;
            }
            PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(seconds, &unk), 1);
        }
    }
}

void DisplayNormalIGT(void) {
    s32 minutes;
    s32 xPos;
    s32 seconds;
    s32 unk;

    if ((gameModeCurrent == 0 || gameModeCurrent == 15) && (gCurrentStage != 8)) {
        minutes = gCurrentStageTime / 1800; //(30 frames * 60 seconds)
        seconds = (gCurrentStageTime % 1800) / 30;
        func_800610A8();
        SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);
        //print ' character
        PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8E8, 1);
        xPos = 220;
        if (minutes > 99) {
            minutes = 99;
            seconds = 59;
        }
        if (minutes < 10) {
            PrintText(0xDC, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8F8, 1);
            xPos = 228;
        }
        //var_v1_2 = ParseIntToBase10(minutes, &unk);
        PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(minutes, &unk), 1);
        xPos = 244;
        if (seconds < 10) {
            PrintText((f32) 0xF4, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8FC, 1);
            xPos = 252;
        }
        PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(seconds, &unk), 1);
    }

}

void DisplayTimerWithMilliseconds(void) { //display IGT with milliseconds
    s32 seconds;
    s32 milliseconds; // Added milliseconds variable
    s32 minutes;
    char timeString[16]; // Assuming a maximum of 99:59.999
    char convertedBuffer[32];  

    if ((gameModeCurrent == 0 || gameModeCurrent == 15) && (gCurrentStage != 8)) {
        s32 totalFrames;
        totalFrames = gCurrentStageTime;
        if (freezeTimer != 0) {
            colorTextWrapper(textCyanColor);
            // totalFrames = gCurrentStageTime;
        } else {
            colorTextWrapper(textOrangeColor);
            // totalFrames = gCurrentStageTime;
        }
        minutes = totalFrames / 1800; // (30 frames * 60 seconds)
        seconds = (totalFrames % 1800) / 30;
        milliseconds = (totalFrames % 30) * 33; // Assuming 30 frames per second
        func_800610A8();
        //SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);

        // Use sprintf to format the time components into a single string
        _sprintf(timeString, "%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        convertAsciiToText(&convertedBuffer, (char*)&timeString);

        // Print the formatted time string
        PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
    }
}

void DisplayTimerWithFrames(void) {
    s32 seconds;
    s32 milliseconds; // Added milliseconds variable
    s32 minutes;
    s32 frames;
    char timeString[16]; // Assuming a maximum of 99:59.999
    char convertedBuffer[32];

    if ((gameModeCurrent == 0 || gameModeCurrent == 15) && (gCurrentStage != 8)) {
        s32 totalFrames;
        if (freezeTimer != 0) {
            colorTextWrapper(textCyanColor);
            totalFrames = gCurrentStageTime;
        } else {
            colorTextWrapper(textOrangeColor);
            totalFrames = gCurrentStageTime;
        }
        totalFrames = gCurrentStageTime;
        frames = totalFrames % 30;
        minutes = totalFrames / 1800; // (30 frames * 60 seconds)
        seconds = (totalFrames % 1800) / 30;
        milliseconds = (totalFrames % 30) * 33; // Assuming 30 frames per second
        func_800610A8();
        
        //SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);

        // Use sprintf to format the time components into a single string
        _sprintf(timeString, "%02d\'%02d\"%02d", minutes, seconds, frames);
        convertAsciiToText(&convertedBuffer, (char*)&timeString);

        // Print the formatted time string
        PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
    }
}

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

        if (gCurrentStage == 8 || toggles[TOGGLE_RTA_TIMER_RESET]) {
            prevCount = osGetCount();
            prevDoorEntryTime = 0;
            totalElapsedCounts = 0;
            elapsedMicroSeconds = 0;
            displayTimeRTA = 0;
            displayTimeIGT = 0;
        }

        //new code to set rng manip stuff
        //if entering bomb land, and set seed option is ON
        if (gCurrentStage == 2 && toggles[TOGGLE_SET_SEED_BL] == 1) { 
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x14B0B9CB;
            *calls = 702;
        } else if (gCurrentStage == 2 && toggles[TOGGLE_SET_SEED_BL] == 2) { 
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x23FB240C;
            *calls = 687;
        } else if (gCurrentStage == 2 && toggles[TOGGLE_SET_SEED_IL_BL]) { 
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x1B684D09;
            *calls = 60;
        } else if (gCurrentStage == 4 && toggles[TOGGLE_SET_SEED_KL]) {
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x1274126A;
            *calls = 1045;
        } else if (gCurrentStage == 5 && toggles[TOGGLE_SET_SEED_GC]) {
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x0C89505D;
            *calls = 10384;
        }

        
        if (gCurrentStage == 7) {
            D_80168DA0 = (u32) gControllerNo;
            D_801749AC = 2;
        } else {
            D_801749AC = 0;
            D_80168DA0 = 1;
        }



        //required 1 liner to match (changed in this since we dont need matching code)
        for (i = 0; i < D_80168DA0; i++) {
            gPlayerActors[i].active = 1;
        }

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

#define STRAIGHT_SHOT 0
#define CURVED_SHOT 1

#define MAX_MOVES 3

//GhostBoss States

#define DO_SHOT 16

#define NULL 0

#define PI 3.141592653589793
#define MAX_DEGREES 360.0
#define SQ(x) ((x)*(x))
extern f32 D_8010BFA4;
extern f32 D_8010BFF8;
extern f64 D_8010C000;
extern f64 D_8010C008;
extern f32 D_8010C010;
extern f32 D_8010C014;
extern f64 D_8010C018;
extern f64 D_8010C020;
extern f64 D_8010C028;
extern f64 D_8010C030;
extern f64 D_8010C038;
extern f64 D_8010C040;
extern f64 D_8010C048;
extern f64 D_8010C050;
extern f64 D_8010C058;
extern f64 D_8010C060;
extern f64 D_8010C068;
extern f64 D_8010C070;
extern f32 D_8010C078;
extern f64 D_8010C080;
extern f64 D_8010C088;
extern f32 D_8010C090;
extern f32 D_8010C094;
void func_800382B4(f32*, f32);
void func_800448C0(Actor*);
void func_80044C30(Actor*, s32);
void func_80044D58(Actor*, s32);
void func_80044EA4(Actor*, f32);
// extern ? D_801749D8;
// extern ? D_801749DC;
//TODO fix type
// extern ? D_80174A14;


typedef struct armsMaybe {
    s32 armActorIDs[15][15]; //static s32 D_801749D8[15];
    // s32 armActorIDs2[15]; //static s32 D_80174A14[15];
} armsMaybe;

//these 3 should maybe be a struct
static armsMaybe D_801749D8;
static s32 D_80174A14[15];
static Vec4f actorPositions[30];
//D_8010C058: [40768000 00000000] 360.0
//D_8010C060: [400921FB 54442D18] 3.141592653589793
#define BOOKS_STILL_ON_ENTRY 0
#define BOOKS_ASSEMBLE_PHASE1 1
#define BOOKS_CREATE_ARMS 2
#define BOOKS_SEND_OUT_ARMS 3
#define BOOKS_ARM_ATTACK 4
#define BOOKS_ARM_ATTACK_COPY 5
#define BOOKS_PULL_ARMS_IN 6
#define BOOKS_PULL_ARMS_IN_COPY 7
#define BOOKS_PHASE1_WALK_BEFORE_ARM_PLANT 8
#define BOOKS_PLANT_ARM 9
#define BOOKS_ARM_SPIN 10
#define BOOKS_PHASE1_WALK_AFTER_ARM_PLANT 11
#define BOOKS_WALK_TO_HOME 12
#define BOOKS_KILL_ARMS 13
#define BOOKS_TRANSFORM_PHASE2 14
#define BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE 15
#define DO_PHASE2_MOVEMENT 16
#define BOOKS_FACE_CENTER_FOR_SHOT 17
#define BOOKS_DO_STRAIGHT_SHOT 18
#define BOOKS_DO_CURVED_SHOT 19
//D_8010C058: [40768000 00000000] 360.0
//D_8010C060: [400921FB 54442D18] 3.141592653589793
void ActorTick_GhostBoss_Hook(Actor* arg0) {
    Actor* sp130;
    Actor* sp128;
    s32 sp11C;
    s32 sp118;
    s32 sp100;
    Actor* spE0;
    s32 spC0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp94;
    f32* sp8C;

    Actor* temp_s0;
    Actor* temp_s0_2;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_6;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f18;
    f32 temp_f20_4;
    f32 temp_f20_5;
    f32 temp_f22_4;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    f32 temp_f2_7;
    f32 var_f20;
    f32 var_f2;
    f32 var_f2_2;
    f32 var_f2_3;

    s32 temp_s0_3;
    s32 temp_s0_5;

    s32 i;

    if (arg0->sizeScalar < 1.0f) {
        arg0->sizeScalar += 0.01f;
        if (arg0->sizeScalar > 1.0f) {
            arg0->sizeScalar = 1.0f;
        }
        func_800312B0(arg0->actorIndex);
        if (((u32) arg0->globalTimer % 20U) == 0) {
            playSoundEffect(0xBA, NULL, NULL, NULL, 0, 0x10);
        }
    }
    //the jump table locations are correct even though rodata is off by 0x04
    switch (arg0->userVariables[2]) {
    case BOOKS_STILL_ON_ENTRY:
        if (arg0->globalTimer >= 0x79U) {
            arg0->pos.y += 20.0f;
            if (arg0->globalTimer == 0x96) {
                arg0->userVariables[2] = BOOKS_ASSEMBLE_PHASE1;
                playSoundEffect(0xB2, NULL, NULL, NULL, 0, 0x10);
            }
        }
        break;
    case BOOKS_ASSEMBLE_PHASE1:
        arg0->pos.y -= D_8010BFF8;
        arg0->pos.z += -4.0f;
        if (arg0->unk_F0 == 0x3C) {
            playSoundEffect(0xB2, NULL, NULL, NULL, 0, 0x10);
        } else if (arg0->unk_F0 == 0xA0) {
            playSoundEffect(0xB3, NULL, NULL, NULL, 0, 0x10);
        }
        
        arg0->unk_F0++;
        
        if (arg0->unk_F0 == 0xB3) {
            arg0->userVariables[2] = BOOKS_CREATE_ARMS;
            arg0->unk_90 = arg0->unk_134[2];
        }
        break;
    case BOOKS_CREATE_ARMS:
        temp_s0 = &gActors[D_801749D8.armActorIDs[0][0]];
        temp_s0_2 = &gActors[D_801749D8.armActorIDs[1][0]];
        temp_s0->pos.x = __cosf((arg0->unk_90 - 90.0f) * 2.0f * PI / 360.0) * 300.0f + arg0->pos.x;
        temp_s0->pos.y = arg0->pos.y + (arg0->unknownPositionThings[0].unk_10 * 0.6f) - (temp_s0->unknownPositionThings[0].unk_10 * 0.125f);
        temp_s0->pos.z = -__sinf((arg0->unk_90 - 90.0f) * 2.0f * PI / 360.0) * 300.0f + arg0->pos.z;
        temp_s0->unk_90 -= 45.0f;
        temp_s0_2->pos.x = __cosf((arg0->unk_90 + 90.0f) * 2.0f * PI / 360.0) * 300.0f + arg0->pos.x;
        temp_s0_2->pos.y = arg0->pos.y + (arg0->unknownPositionThings[0].unk_10 * 0.6f) - (temp_s0_2->unknownPositionThings[0].unk_10 * 0.125f);
        temp_s0_2->pos.z = -__sinf((arg0->unk_90 + 90.0f) * 2.0f * PI / 360.0) * 300.0f + arg0->pos.z;
        temp_s0_2->unk_90 = arg0->unk_90 + 45.0f;
        arg0->userVariables[0] = -1;
        arg0->userVariables[1] = -1;
        func_80044C30(arg0, 2);
        func_800448C0(arg0);
        arg0->userVariables[2] = BOOKS_SEND_OUT_ARMS;
        playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
        break;
    case BOOKS_SEND_OUT_ARMS:
        temp_s0 = &gActors[D_801749D8.armActorIDs[0][0]];
        temp_s0_2 = &gActors[D_801749D8.armActorIDs[1][0]];
        temp_f2 = temp_s0->pos.y;
        temp_s0->pos.x += 20.0f * __cosf(temp_s0->unk_90 * 2.0f * 360.0 / PI);
        temp_s0->pos.y = temp_f2 - (temp_f2 / 10.0f);
        temp_s0->pos.z += 20.0f * -__sinf(temp_s0->unk_90 * 2.0f * 360.0 / PI);
        temp_s0_2->pos.x += 20.0f * __cosf(temp_s0_2->unk_90 * 2.0f * 360.0 / PI);
        temp_s0_2->pos.y -= (temp_s0_2->pos.y / 10.0f);
        temp_s0_2->pos.z += 20.0f * -__sinf(temp_s0_2->unk_90 * 2.0f * 360.0 / PI);
        func_80044C30(arg0, 2);
        func_800448C0(arg0);
        if (func_80044E80(arg0, 0) == 8) {
            arg0->userVariables[2] = BOOKS_ARM_ATTACK;
            arg0->userVariables[4] += 1;
            if ((f32) (Random(0, 60000) % 100) < (arg0->unk_164 * 100.0f)) {
                arg0->unk_134[7] = 2.0f;
            } else {
                arg0->unk_134[7] = 1.0f;
            }
            playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
        }
        break;
    case BOOKS_ARM_ATTACK:
    case BOOKS_ARM_ATTACK_COPY:
        if (func_80044E80(arg0, 5 - arg0->userVariables[2]) == 0) {
            arg0->unk_134[6] += 1.0f;
            if ((arg0->unk_134[6] == 30.0f) || (arg0->unk_124 != arg0->userVariables[4])) {
                temp_f0_2 = arg0->pos.x - gPlayerOnePointer->pos.x;
                temp_f2_3 = arg0->pos.z - gPlayerOnePointer->pos.z;
                arg0->unk_134[6] = 0;
                if ((arg0->unk_124 == arg0->userVariables[4]) || (arg0->userVariables[3] == 0)) {
                    arg0->userVariables[4] = 0;
                    arg0->userVariables[2] = BOOKS_PULL_ARMS_IN_COPY;
                } else if (arg0->position._f32.x < ((SQ(temp_f0_2)) + (SQ(temp_f2_3)))) {
                    arg0->userVariables[2] = BOOKS_PULL_ARMS_IN;
                } else if (arg0->userVariables[2] == BOOKS_ARM_ATTACK) {
                    arg0->userVariables[2] = BOOKS_ARM_ATTACK_COPY;
                    arg0->userVariables[4]++;
                    if ((Random(0, 65336) % 256) < (arg0->unk_164 * 256.0f)) {
                        arg0->unk_134[7] = 2.0f;
                    } else {
                        arg0->unk_134[7] = 1.0f;
                    }
                    playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
                } else {
                    arg0->userVariables[2] = BOOKS_ARM_ATTACK;
                    arg0->userVariables[4]++;
                    if ((Random(0, 65336) % 256) < (arg0->unk_164 * 256.0f)) {
                        arg0->unk_134[7] = 2.0f;
                    } else {
                        arg0->unk_134[7] = 1.0f;
                    }
                    playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
                }
            } else {
                goto block_53;
            }
        } else {
            if ((arg0->userVariables[2] - 4) != 0) {
                temp_s0 = &gActors[D_801749D8.armActorIDs[1][0]];
            } else {
                temp_s0 = &gActors[D_801749D8.armActorIDs[0][0]];
            }
            if (arg0->unk_124 == arg0->userVariables[4]) {
                sp118 = (s32) (arg0->unk_134[7] * 2.0f);
            } else {
                sp118 = (s32) arg0->unk_134[7];
            }
            sp11C = 0;
            if (sp118 > 0) {
                sp8C = &temp_s0->unk_90;
loop_48:
                if (func_80044E80(arg0, arg0->userVariables[2] - 4) < 9) {
                    temp_f0_3 = temp_s0->pos.y;
                    temp_s0->pos.y = temp_f0_3 - (temp_f0_3 / 10.0f);
                    var_f2 = 6.0f / sp118;
                } else {
                    var_f2 = 2.0f / sp118;
                }
                func_8002D36C(sp8C, CalcAngleBetween2DPoints(temp_s0->pos.x, temp_s0->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z), var_f2);
                temp_s0->pos.x += 20.0f * __cosf((f32) (((f64) (temp_s0->unk_90 * 2.0f) * PI) / 360.0));
                temp_s0->pos.z += 20.0f * -__sinf((f32) (((f64) (temp_s0->unk_90 * 2.0f) * PI) / 360.0));
                func_80044C30(arg0, arg0->userVariables[2] - 4);
                func_80044D58(arg0, 5 - arg0->userVariables[2]);
                if (func_80044E80(arg0, 5 - arg0->userVariables[2]) != 0) {
                    sp11C++;
                    if (sp11C != sp118) {
                        goto loop_48;
                    }
                }
            }
block_53:
            func_800448C0(arg0);
            if ((D_8017499C % 24) == 0) {
                playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
            }
        }
        break;
    case BOOKS_PULL_ARMS_IN:
    case BOOKS_PULL_ARMS_IN_COPY:
        temp_f20_4 = arg0->pos.x - gPlayerOnePointer->pos.x;
        temp_f22_4 = arg0->pos.z - gPlayerOnePointer->pos.z;
        temp_s0_3 = func_80044E80(arg0, 0);
        sp100 = func_80044E80(arg0, 1);
        if (temp_s0_3 != 0) {
            func_80044D58(arg0, 0);
        }
        if (sp100 != 0) {
            func_80044D58(arg0, 1);
        }
        func_800448C0(arg0);
        if ((temp_s0_3 == 0) && (sp100 == 0)) {
            if (arg0->userVariables[3] == 0) {
                arg0->userVariables[2] = BOOKS_KILL_ARMS;
                arg0->userVariables[4] = 0;
            } else if (arg0->userVariables[2] == BOOKS_PULL_ARMS_IN_COPY) {
                arg0->userVariables[2] = BOOKS_PHASE1_WALK_BEFORE_ARM_PLANT;
                arg0->unk_EC = 2;
                arg0->unk_F0 = 0;
            } else if (((SQ(temp_f20_4)) + (SQ(temp_f22_4))) < arg0->position._f32.x) {
                arg0->userVariables[2] = BOOKS_SEND_OUT_ARMS;
                playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
            }
        } else if (((s32) D_8017499C % 24) == 0) { //every 24 frames play books regen noise if regenerating health
            playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
        }
        break;
    case BOOKS_PHASE1_WALK_BEFORE_ARM_PLANT:
        arg0->pos.z += 20.0f;
        gActors[D_801749D8.armActorIDs[0][0]].pos.z += 20.0f;
        gActors[D_801749D8.armActorIDs[1][0]].pos.z += 20.0f;
        if (arg0->pos.z >= 0.0f) {
            if (arg0->pos.x < gPlayerOnePointer->pos.x) {
                arg0->unk_120 = 1;
            } else {
                arg0->unk_120 = 0;
            }
            if (arg0->pos.z < gPlayerOnePointer->pos.z) {
                if ((Random(0, 30000) % 300) < 120) {
                    arg0->unk_134[5] = 0.5f;
                } else {
                    arg0->unk_134[5] = 1.0f;
                }
            } else if ((Random(0, 30000) % 300) < 120) {
                arg0->unk_134[5] = -0.5f;
            } else {
                arg0->unk_134[5] = -1.0f;
            }
            arg0->unk_16C = ((Random(0, 99999) % 2) * 2) - 1;
            arg0->userVariables[2] = BOOKS_PLANT_ARM;
            arg0->userVariables[arg0->unk_120] = -1;
            func_80044C30(arg0, arg0->unk_120);
            temp_f12 = (f32) (((f64) ((((f32) (1 - (arg0->unk_120 * 2)) * arg0->unk_134[5] * 90.0f) + CalcAngleBetween2DPoints(gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z, arg0->pos.x, arg0->pos.z)) * 2.0f) * PI) / 360.0);
            arg0->unk_134[3] = CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, (__cosf(temp_f12) * 300.0f) + gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z - (__sinf(temp_f12) * 300.0f));
            arg0->unk_134[4] = 0.0f;
        } else {
            arg0->unk_F0++;
            if ((arg0->unk_EC == 2) && (arg0->unk_F0 == 0xC)) {
                arg0->unk_EC = 3;
                arg0->unk_F0 = 0;
            }
        }
        break;
    case BOOKS_PLANT_ARM:
        temp_s0 = &gActors[D_801749D8.armActorIDs[arg0->unk_120][0]];
        temp_s0_2 = &gActors[D_801749D8.armActorIDs[arg0->unk_120][1]]; //unsure on this. above line is m2c output
        for (i = 0; i < 3; i++) {
            if (func_80044E80(arg0, arg0->unk_120) < 9) {
                var_f2_2 = 9.0f;
                temp_s0->pos.y -= (temp_s0->pos.y / 10.0f);
            } else {
                var_f2_2 = 3.0f;
            }
            
            func_8002D36C(&temp_s0->unk_90, arg0->unk_134[3], var_f2_2);
            temp_s0->pos.x += 20.0f * __cosf((f32) (((f64) (temp_s0->unk_90 * 2) * PI) / 360.0));
            temp_s0->pos.z += 20.0f * -__sinf((f32) (((f64) (temp_s0->unk_90 * 2) * PI) / 360.0));
            func_80044C30(arg0, arg0->unk_120);
        }
        
        func_800448C0(arg0);
        arg0->userVariables[4]++;
        
        if (arg0->userVariables[4] < 20) {
            arg0->unk_134[4] += 1.0f;
            temp_s0_2->pos.z += 10.0f;
        }
        if (func_80044E80(arg0, arg0->unk_120) == 15) {
            arg0->userVariables[2] = BOOKS_ARM_SPIN;
            arg0->userVariables[4] = 0;
            arg0->unk_134[0] = 0.0f;
            playSoundEffect(0xB5, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
        }
        break;
    case BOOKS_ARM_SPIN:
        arg0->userVariables[4]++;
        var_f20 = arg0->unk_16C * (arg0->position._f32.y * (f32) ((arg0->unk_120 * 2) - 1) * arg0->unk_134[5]);
        if (arg0->userVariables[4] < 8) {
            var_f20 *= (f32) ((s32) (arg0->userVariables[4] + 1) / 8);
        } else if (arg0->userVariables[4] == 8) {
            playSoundEffect(0xB5, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
        }
        
        var_f2_3 = arg0->unk_134[0] + var_f20;
        
        if (var_f2_3 > 360.0f) {
            var_f20 = 360.0f - arg0->unk_134[0];
            var_f2_3 = arg0->unk_134[0] + var_f20;
        }
        
        if (var_f2_3 < -360.0f) {
            var_f20 = -360.0f - arg0->unk_134[0];
            var_f2_3 = arg0->unk_134[0] + var_f20;
        }
        
        arg0->unk_134[0] = var_f2_3;
        arg0->unk_90 += var_f20;
        WrapDegrees(&arg0->unk_90);
        func_80044EA4(arg0, var_f20);
        temp_f0_6 = arg0->unk_134[0];
        
        if ((temp_f0_6 >= 360.0f) || (temp_f0_6 <= -360.0f)) {
            arg0->userVariables[2] = BOOKS_PHASE1_WALK_AFTER_ARM_PLANT;
            arg0->userVariables[4] = 0;
            arg0->unk_90 = arg0->unk_134[2];
            arg0->unk_134[6] = 0.0f;
        }
        break;
    case BOOKS_PHASE1_WALK_AFTER_ARM_PLANT:
        for (i = 0; i < 3; i++) {
            func_80044D58(arg0, arg0->unk_120);
        }
        
        func_800448C0(arg0);
        temp_s0 = &gActors[D_801749D8.armActorIDs[arg0->unk_120][0]];
        
        if (arg0->userVariables[4]++ < 0x14) {
            arg0->unk_134[4] -= 1.0f;
            temp_s0->pos.z -= 10.0f;
        }
        if (func_80044E80(arg0, arg0->unk_120) == 0) {
            arg0->userVariables[2] = BOOKS_WALK_TO_HOME;
            arg0->userVariables[4] = 0;
            arg0->unk_134[4] = 0.0f;
        }
        break;
    case BOOKS_WALK_TO_HOME:
        temp_s0 = &gActors[D_801749D8.armActorIDs[0][0]];
        temp_s0_2 = &gActors[D_801749D8.armActorIDs[1][0]];
        arg0->pos.z -= 20.0f;
        temp_s0->pos.z -= 20.0f;
        temp_s0_2->pos.z -= 20.0f;
        temp_f2_4 = arg0->unk_134[1];
        
        if (arg0->pos.z <= temp_f2_4) {
            arg0->pos.z = temp_f2_4;
            if (arg0->userVariables[3] == 0) {
                arg0->userVariables[4] = 0;
                arg0->userVariables[2] = BOOKS_KILL_ARMS;
            } else {
                arg0->userVariables[2] = BOOKS_SEND_OUT_ARMS;
                playSoundEffect(0xB4, &arg0->pos.x, &arg0->pos.y, &arg0->pos.z, 0, 0);
                arg0->userVariables[4] = 0;
                arg0->userVariables[arg0->unk_120] = -1;
                func_80044C30(arg0, arg0->unk_120);
                arg0->unk_EC = 0;
                arg0->unk_F0 = 0xB3;
            }
        } else {
            if (arg0->unk_EC == 2) {
                arg0->unk_F0 -= 1;
            } else {
                arg0->unk_F0 += 0x27;
            }
            if ((arg0->unk_EC == 3) && (arg0->pos.z <= (arg0->unk_134[1] + 240.0f))) {
                arg0->unk_EC = 2;
                arg0->unk_F0 = 0xB;
            }
        }
        break;
    case BOOKS_KILL_ARMS:
        arg0->userVariables[4]++;
        if (arg0->userVariables[4] == 1) {
            func_800313BC(D_801749D8.armActorIDs[0][0], 180.0f);
            func_800313BC(D_801749D8.armActorIDs[1][0], 0.0f);
            for (i = 0; i < 4; i++) {
                func_80031518(&gActors[D_801749D8.armActorIDs[0][i]]);
                func_80031518(&gActors[D_801749D8.armActorIDs[1][i]]);
            }
        } else if (arg0->userVariables[4] == 0x1E) {
            arg0->userVariables[4] = 0;
            arg0->userVariables[2] = BOOKS_TRANSFORM_PHASE2;
            arg0->userVariables[0] = 0;
            arg0->unk_EC = 1;
            arg0->unk_F0 = 0;
            playSoundEffect(0xB2, NULL, NULL, NULL, 0, 0x10);
        }
        break;
    case BOOKS_TRANSFORM_PHASE2:
        if (arg0->unk_F0 == 0x42) {
            playSoundEffect(0xB2, NULL, NULL, NULL, 0, 0x10);
        } else if (arg0->unk_F0 == 0xB4) {
            playSoundEffect(0xB3, NULL, NULL, NULL, 0, 0x10);
        }
        
        arg0->unk_F0++;
        
        if (arg0->unk_F0 == 0xC7) {
            arg0->userVariables[2] = BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE;
            arg0->unknownPositionThings[0].unk_10 = 350.0f;
            arg0->tYPos = 350.0f;
            arg0->unk_120 = Random(0, 99999999) % MAX_MOVES;
            playBGM(0x10);
        }
        break;
    case BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE:
        for (i = 1; i < 0x21; i++) {
            temp_s0_5 = Random(0, 99999999); //99,999,999
            temp_f2_5 = CalculateAngleOfVector(arg0->pos.x, -arg0->pos.z) + (temp_s0_5 % 300) + 30.0f;
            arg0->unk_134[3] = __cosf(temp_f2_5 * 2 * PI / 360.0) * 1800.0f;
            arg0->unk_134[4] = -__sinf(temp_f2_5 * 2 * PI / 360.0) * 1800.0f;
            if (IsAngleWithinTolerance(CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z), CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, arg0->unk_134[3], arg0->unk_134[4]), 30.0f) == 0) {
                continue;
            }
            break;
        }
        arg0->userVariables[2] = DO_PHASE2_MOVEMENT; //actor state
        break;
    case DO_PHASE2_MOVEMENT:
        temp_f2_6 = arg0->pos.x - arg0->unk_134[3];
        temp_f18 = CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, arg0->unk_134[3], arg0->unk_134[4]);
        temp_f12_2 = arg0->pos.z - arg0->unk_134[4];
        if (((SQ(temp_f2_6)) + (SQ(temp_f12_2))) < (2.0f * (SQ(arg0->unk_160)))) {
            arg0->unk_120--;
            arg0->unk_94 = 0.0f;
            if (arg0->unk_120 != 0) {
                arg0->userVariables[2] = BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE;
            } else {
                arg0->userVariables[2] = BOOKS_FACE_CENTER_FOR_SHOT;
                if ((Random(0, 99999) % 100) < 50) {
                    arg0->unk_120 = STRAIGHT_SHOT;
                } else {
                    arg0->unk_120 = CURVED_SHOT;
                }
                if (arg0->unk_120 == 0) {
                    arg0->unk_134[5] = CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z);
                } else {
                    arg0->unk_134[5] = CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z) - 30.0f;
                    WrapDegrees(&arg0->unk_134[5]);
                }
            }
        } else {
            func_8002F6DC(&arg0->unk_90, temp_f18);
            arg0->unk_94 += arg0->unk_15C;
            func_800382B4(&arg0->unk_94, arg0->unk_160);
        }
        break;
    case BOOKS_FACE_CENTER_FOR_SHOT:
        func_8002F6DC(&arg0->unk_90, arg0->unk_134[5]);
        if (arg0->unk_90 == arg0->unk_134[5]) {
            if (arg0->unk_120 == 0) {
                arg0->userVariables[2] = BOOKS_DO_STRAIGHT_SHOT;
                arg0->userVariables[4] = Random(arg0->unk_12C, arg0->unk_130);
            } else {
                arg0->userVariables[2] = BOOKS_DO_CURVED_SHOT;
                arg0->userVariables[4] = 0;
            }
        }
        break;
    case BOOKS_DO_STRAIGHT_SHOT:
        spAC = arg0->pos.x - gPlayerOnePointer->pos.x;
        spA8 = arg0->pos.z - gPlayerOnePointer->pos.z;
        if ((gPlayerOnePointer->amountLeftToShoot != 0) && (arg0->userVariables[4] >= 4)) {
            func_8002F6DC(&arg0->unk_90, CalcAngleBetween2DPoints(arg0->pos.x, arg0->pos.z, gPlayerOnePointer->pos.x, gPlayerOnePointer->pos.z));
        }
        if (!(arg0->globalTimer & 3)) {
            if (Actor_Init(0x40, (__cosf(arg0->unk_90 * 2.0f * PI / 360.0) * 280.0f) + arg0->pos.x, arg0->pos.y + 100.0f, arg0->pos.z - (__sinf(arg0->unk_90 * 2.0f * PI / 360.0) * 280.0f), arg0->unk_90, arg0->unk_F4, arg0->unk_F8, arg0->unk_FC, arg0->unk_100, arg0->unk_104, arg0->unk_108, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0) != -1) {
                playSoundEffect(0xBB, NULL, NULL, NULL, 0, 0x10);
                arg0->userVariables[4] -= 1;
            }
        }
        if ((arg0->userVariables[4] == 0) || (((SQ(spAC)) + (SQ(spA8))) < 640000.0f)) {
            arg0->userVariables[2] = BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE;
            arg0->unk_120 = Random(0, 99999999) % MAX_MOVES;
        }
        break;
    case BOOKS_DO_CURVED_SHOT:
        spA4 = arg0->pos.x - gPlayerOnePointer->pos.x;
        spA0 = arg0->pos.z - gPlayerOnePointer->pos.z;
        if (!(arg0->globalTimer & 3)) {
            if (Actor_Init(0x40, (__cosf(arg0->unk_90 * 2.0f * PI / 360.0) * 280.0f) + arg0->pos.x, arg0->pos.y + 100.0f, arg0->pos.z - (__sinf(arg0->unk_90 * 2.0f * PI / 360.0) * 280.0f), arg0->unk_90, arg0->unk_F4, arg0->unk_F8, arg0->unk_FC, arg0->unk_100, arg0->unk_104, arg0->unk_108, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0) != -1) {
                playSoundEffect(0xBB, NULL, NULL, NULL, 0, 0x10);
            }
        }
        arg0->unk_90 += 0.803571f;
        WrapDegrees(&arg0->unk_90);
        arg0->userVariables[4]++;
        if ((arg0->userVariables[4] == 0x70) || (gPlayerOnePointer->amountLeftToShoot != 0) || ((SQ(spA4) + SQ(spA0)) < 640000.0f)) {
            arg0->userVariables[2] = BOOKS_CHOOSE_INITIAL_PHASE2_ANGLE;
            arg0->unk_120 = Random(0, 99999999) % MAX_MOVES;
        }
        break;
    }
    func_800382F4(arg0);
    func_800360E4(arg0);
}