.include "macro.inc"

glabel osViRepeatLine
ADDIU          $sp, $sp, -0x18
SW             $s0, 0x0010 ($sp)
SW             $ra, 0x0014 ($sp)
JAL            __osDisableInt
ADDU           $s0, $a0, $zero
ANDI           $s0, $s0, 0x00FF
BEQZ           $s0, label1
ADDU           $a0, $v0, $zero
LUI            $v1, 0x8011
LW             $v1, 0x9E94 ($v1)
LHU            $v0, 0x0000 ($v1)
J              label0
ORI            $v0, $v0, 0x0040
label1:
LUI            $v1, 0x8011
LW             $v1, 0x9E94($v1)
LHU            $v0, 0x0000 ($v1)
ANDI           $v0, $v0, 0xFFBF
label0:
JAL            __osRestoreInt
SH             $v0, 0x0000 ($v1)
LW             $ra, 0x0014 ($sp)
LW             $s0, 0x0010 ($sp)
JR             $ra
ADDIU          $sp, $sp, 0x18
