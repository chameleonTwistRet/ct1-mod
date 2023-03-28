.include "macro.inc"

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
