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