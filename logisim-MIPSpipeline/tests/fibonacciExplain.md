addi 0x20010000 // reg1 = 0
addi 0x20020001 // reg2 = 1
addi 0x20030001 // reg3 = 1
addi 0x20050010 // reg5 = 16
loop:
add  0x00220820 // reg1 = reg1+reg2
add  0x00202020 // reg4 = reg1
add  0x00400820 // reg1 = reg2
add  0x00801020 // reg2 = reg4
addi 0x20630001 // reg3 = reg3+1
bne  0x1465fffa // reg3 != reg5 ? loop : continue
continue: