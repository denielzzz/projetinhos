addi 0x20010001 // r1 = 1
sw   0xac010001 // m[1] = r1
lw   0x8c020001 // r2 = m[1]
lw   0x8c430000 // r3 = m[r2]
lw   0x8c640000 // r4 = m[r3]