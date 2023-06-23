.include "macro.inc"
.set noreorder

glabel loadEnemyObjectsHook
mtc1 $a2, $f12
addiu $t7, $t7, 0xAC68
lui $a2, %hi(loadEnemiesBool)
addiu $a2, $a2, %lo(loadEnemiesBool)
lw $a2, 0 ($a2)
bnezl $a2, exit
addu $a1, $zero, $zero
exit:
JR $ra
nop

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
