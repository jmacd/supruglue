;;  SystemOnChipSuspend calls SLP
    .global SystemOnChipSuspend
SystemOnChipSuspend:
    SLP 1
    JMP r3.w2
