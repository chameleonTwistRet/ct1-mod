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
