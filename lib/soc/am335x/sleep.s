;;  SystemOnChipSleep calls SLP
    .global SystemOnChipSleep
SystemOnChipSleep:
    SLP 1
    JMP r3.w2
