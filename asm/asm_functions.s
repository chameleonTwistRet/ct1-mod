.include "macro.inc"
.set noreorder

glabel loadEnemyObjectsHook
    LUI $t0, %hi(loadEnemiesBool)
    ADDIU $t0, $t0, %lo(loadEnemiesBool)
    LW $t1, 0x0000 ($t0)
    ORI $t2, $zero, 0x0063 /* crown ID */
    BEQ $t2, $a1, obj_return
    ORI $t2, $zero, 0x0062 /* big yellow heart */
    BEQ $t2, $a1, obj_return
    ORI $t2, $zero, 0x0061 /* big heart ID */
    BEQ $t2, $a1, obj_return
    ORI $t2, $zero, 0x005F /* small heart ID */
    BEQ $t2, $a1, obj_return
    NOP
    BNEZL $t1, obj_return
    ADDIU $a1, $zero, 0x0 /* No enemies or objects appear */
    NOP
obj_return:
    ADDIU $t7, $t7, 0xAC68
    J 0x8002D668 /* +0x08 from hook */
    NOP

glabel gVideoThreadProcessHook
    gVideoThreadBoolCheckLoop:
    LUI $t0, %hi(isSaveOrLoadActive)
    LW $t0, %lo(isSaveOrLoadActive) ($t0)
    BNEZ $t0, gVideoThreadBoolCheckLoop
    NOP
    JAL osRecvMesg
    OR $a2, $s0, $zero
    J 0x80084C10
    NOP


glabel __osDpDeviceBusy
LUI $t6, 0xA410
LW $a0, 0x000C ($t6)
ADDIU $sp, $sp, -0x08
ANDI $t7, $a0, 0x0100
BEQZ $t7, dpRet
NOP
B dpExit
ADDIU $v0, $zero, 1
dpRet:
OR $v0, $zero, $zero
dpExit:
jr $ra
ADDIU $sp, $sp, 0x08


# MIPS assembly function for memory copy in chunks of 8 bytes
# using LD and SD instructions

# Arguments:
# $a0 = destination address
# $a1 = source address
# $a2 = number of bytes to copy

glabel optimized_memcpy
    addu $t0, $zero, $zero      # Counter for remaining bytes
    addu $t1, $zero, $a2        # Number of bytes to copy
    addu $t2, $a0, $t1          # End destination address
    subu $t2, $t2, 1            # Last destination address
    
    # Loop to copy in chunks of 8 bytes
    ld_loop:
        subu $t0, $t1, 8         # Calculate remaining bytes
        blez $t0, byte_copy      # Branch if less than 8 bytes remaining
        .word 0xDCAB0000    #ld $t3, 0($a1)           # Load 8-byte chunk from source
        .word 0xFC8B0000    #sd $t3, 0($a0)           # Store 8-byte chunk to destination
        
        addiu $a0, $a0, 8        # Increment destination address by 8
        addiu $a1, $a1, 8        # Increment source address by 8
        
        
        j ld_loop               # Jump to the beginning of the loop
            subu $t1, $t1, 8         # Decrement remaining bytes by 8
    
    # Copy remaining bytes byte by byte
    byte_copy:
        subu $t0, $t0, 1         # Decrement remaining bytes by 1
        blez $t0, end_memcpy    # Branch if no more bytes remaining
            lbu $t3, 0($a1)          # Load 1 byte from source
        sb $t3, 0($a0)           # Store 1 byte to destination
        addiu $a0, $a0, 1        # Increment destination address by 1
        
        
        j byte_copy             # Jump to the beginning of the loop
            addiu $a1, $a1, 1        # Increment source address by 1
    
    end_memcpy:
        jr $ra                   # Return
        nop

glabel setTimerParametersBool
    ori $t1, $zero, 1
    lui $t2, %hi(timerParametersBool)
    sw $t1, %lo(timerParametersBool) ($t2) /*set freeze on */
    lw $t7, 0x0048 ($t3)
    sw $t0, 0x0000 ($v1)
    J 0x800C11D0
    nop

glabel setFreezeTimerAsm
    J setFreezeTimerC
    nop

glabel enemyOnTongueHook
    lui $t4, %hi(freezeTimer)
    ori $t5, $zero, 0x28 /*40 frames*/
    sw $t5, %lo(freezeTimer) ($t4)


    lui $t4, %hi(gCurrentZone)
    lw $t4, %lo(gCurrentZone) ($t4)
    lui $t5, %hi(zoneExitID)
    sw $t4, %lo(zoneExitID) ($t5)

    lui $t4, %hi(totalElapsedCounts)
    ld $t4, %lo(totalElapsedCounts) ($t4)
    lui $t5, %hi(displayTimeRTA)
    sd $t4, %lo(displayTimeRTA) ($t5)

    lui $t4, %hi(gCurrentStageTime)
    lw $t4, %lo(gCurrentStageTime) ($t4)
    lui $t5, %hi(displayTimeIGT)
    sd $t4, %lo(displayTimeIGT) ($t5)
    sw $t8, 0x04B4 ($s1)
    J 0x80032680
    sw $t0, 0x0014 ($sp)

glabel removeEnemiesFromMouthHook
    lui $a0, %hi(freezeTimer)
    ori $t8, $zero, 0x28 /*40 frames*/
    sw $t8, %lo(freezeTimer) ($a0)


    lui $a0, %hi(gCurrentZone)
    lw $a0, %lo(gCurrentZone) ($a0)
    lui $t8, %hi(zoneExitID)
    sw $a0, %lo(zoneExitID) ($t8)

    lui $a0, %hi(totalElapsedCounts)
    ld $a0, %lo(totalElapsedCounts) ($a0)
    lui $t8, %hi(displayTimeRTA)
    sd $a0, %lo(displayTimeRTA) ($t8)

    lui $a0, %hi(gCurrentStageTime)
    lw $a0, %lo(gCurrentStageTime) ($a0)
    lui $t8, %hi(displayTimeIGT)
    sd $a0, %lo(displayTimeIGT) ($t8)


    lui	$a0, 0x8017
    lw $t8, 0x0000 ($t0)
    J 0x80035930
    sw $zero, 0x0004 ($t8)



glabel osSetCount
/* BB650 800E0250 40024800 */  mtc0       $a0, $9 # handwritten instruction
/* BB658 800E0258 00000000 */  nop
/* BB65C 800E025C 00000000 */  nop
/* BB654 800E0254 03E00008 */  jr         $ra
nop

glabel endStageCodeAsm
lui $at, 0x8017
sw $t6, 0x4980 ($at)
jal endStageCode
nop
j 0x80037188
nop

glabel endStageCodeAsm2
jal endStageCode
nop
j 0x8004B474
nop

glabel storeFirstEntry
lw $t7, 0x0004 ($v0)
lui $v1, %hi(isFirstZoneCopy)
ori $v1, $v1, %lo(isFirstZoneCopy)
sw $t7, 0x0000 ($v1)
sw $zero, 0x0004 ($v0)
MFLO $t7
J 0x800C10CC
NOP


glabel getActorHitWithSpatActor
addiu $sp, $sp, -0x20
swc1 $f12, 0x0010 ($sp)
swc1 $f14, 0x0014 ($sp)

LW $t0, 0x0000 ($a0)
ORI $t1, $zero, 0x0026
BNE $t0, $t1, didntHitCake
NOP

jal incrementCakeHitCounter
nop

didntHitCake:
lwc1 $f12, 0x0010 ($sp)
lwc1 $f14, 0x0014 ($sp)
neg.s $f14, $f14
j 0x80037914
addiu $sp, $sp, 0x20
