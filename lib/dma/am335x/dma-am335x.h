// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_DMA_AM335X_DMA_H
#define LIB_DMA_AM335X_DMA_H

#include "lib/dma/dma-defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EDMA_BASE ((volatile uint32_t *)(0x49000000))
#define EDMA_DRAE1 (0x348 / WORDSZ)  // DMA Region Access Enable Register for Region 1 Section 11.4.1.20
#define EDMA_DRAEH1 (0x34C / WORDSZ) // DMA Region Access Enable Register High for Region 1 Section 11.4.1.21

#define EDMA_EMR (0x300 / WORDSZ)   // Event Missed Register Section 11.4.1.9
#define EDMA_EMRH (0x304 / WORDSZ)  // Event Missed Register High Section 11.4.1.10
#define EDMA_EMCR (0x308 / WORDSZ)  // Event Missed Clear Register Section 11.4.1.11
#define EDMA_EMCRH (0x30C / WORDSZ) // Event Missed Clear Register High Section 11.4.1.12

#define EDMA_CCERR (0x318 / WORDSZ)    // EDMA3CC Error Register Section 11.4.1.15
#define EDMA_EEVAL (0x320 / WORDSZ)    // EDMA3CC EEVAL
#define EDMA_CCERRCLR (0x31C / WORDSZ) // EDMA3CC Error Clear Register Section 11.4.1.16

#define EDMAREG_ER 0x00    // Event Register Section 11.4.1.91
#define EDMAREG_ERH 0x04   // Event Register High Section 11.4.1.92
#define EDMAREG_ECR 0x08   // Event Clear Register Section 11.4.1.93
#define EDMAREG_ECRH 0x0C  // Event Clear Register High Section 11.4.1.94
#define EDMAREG_ESR 0x10   // Event Set Register Section 11.4.1.95
#define EDMAREG_ESRH 0x14  // Event Set Register High Section 11.4.1.96
#define EDMAREG_CER 0x18   // Chained Event Register Section 11.4.1.97
#define EDMAREG_CERH 0x1C  // Chained Event Register High Section 11.4.1.98
#define EDMAREG_EER 0x20   // Event Enable Register Section 11.4.1.99
#define EDMAREG_EERH 0x24  // Event Enable Register High Section 11.4.1.100
#define EDMAREG_EECR 0x28  // Event Enable Clear Register Section 11.4.1.101
#define EDMAREG_EECRH 0x2C // Event Enable Clear Register High Section 11.4.1.102
#define EDMAREG_EESR 0x30  // Event Enable Set Register Section 11.4.1.103
#define EDMAREG_EESRH 0x34 // Event Enable Set Register High Section 11.4.1.104
#define EDMAREG_SER 0x38   // Secondary Event Register Section 11.4.1.105
#define EDMAREG_SERH 0x3C  // Secondary Event Register High Section 11.4.1.106
#define EDMAREG_SECR 0x40  // Secondary Event Clear Register Section 11.4.1.107
#define EDMAREG_SECRH 0x44 // Secondary Event Clear Register High Section 11.4.1.108
#define EDMAREG_IER 0x50   // Interrupt Enable Register Section 11.4.1.109
#define EDMAREG_IERH 0x54  // Interrupt Enable Register High Section 11.4.1.110
#define EDMAREG_IECR 0x58  // Interrupt Enable Clear Register Section 11.4.1.111
#define EDMAREG_IECRH 0x5C // Interrupt Enable Clear Register High Section 11.4.1.112
#define EDMAREG_IESR 0x60  // Interrupt Enable Set Register Section 11.4.1.113
#define EDMAREG_IESRH 0x64 // Interrupt Enable Set Register High Section 11.4.1.114
#define EDMAREG_IPR 0x68   // Interrupt Pending Register Section 11.4.1.115
#define EDMAREG_IPRH 0x6C  // Interrupt Pending Register High Section 11.4.1.116
#define EDMAREG_ICR 0x70   // Interrupt Clear Register Section 11.4.1.117
#define EDMAREG_ICRH 0x74  // Interrupt Clear Register High Section 11.4.1.118
#define EDMAREG_IEVAL 0x78 // Interrupt Evaluate Register Section 11.4.1.119
#define EDMAREG_QER 0x80   // QDMA Event Register Section 11.4.1.120
#define EDMAREG_QEER 0x84  // QDMA Event Enable Register Section 11.4.1.121
#define EDMAREG_QEECR 0x88 // QDMA Event Enable Clear Register Section 11.4.1.122
#define EDMAREG_QEESR 0x8C // QDMA Event Enable Set Register Section 11.4.1.123
#define EDMAREG_QSER 0x90  // QDMA Secondary Event Register Section 11.4.1.124
#define EDMAREG_QSECR 0x94 // QDMA Secondary Event Clear Register Section 11.4.1.125

#define EDMA_DCHMAP_N(N) ((0x100 + (N << 2)) / WORDSZ)
#define EDMA_DMAQNUM_0 (0x240 / WORDSZ) // DMA Queue Number Register 0
#define EDMA_DMAQNUM_1 (0x244 / WORDSZ) // DMA Queue Number Register 1
#define EDMA_DMAQNUM_2 (0x248 / WORDSZ) // DMA Queue Number Register 2
#define EDMA_DMAQNUM_3 (0x24C / WORDSZ) // DMA Queue Number Register 3
#define EDMA_DMAQNUM_4 (0x250 / WORDSZ) // DMA Queue Number Register 4
#define EDMA_DMAQNUM_5 (0x254 / WORDSZ) // DMA Queue Number Register 5
#define EDMA_DMAQNUM_6 (0x258 / WORDSZ) // DMA Queue Number Register 6
#define EDMA_DMAQNUM_7 (0x25C / WORDSZ) // DMA Queue Number Register 7
#define SHADOW1(reg) ((0x2200 + reg) / WORDSZ)

// (TRM 11.3.3.1)
#define EDMA_PARAM_OFFSET (0x4000 / WORDSZ)
#define EDMA_PARAM_SIZE sizeof(edmaParam) // 32 bytes
#define EDMA_PARAM_NUM 256

// EDMA PARAM registers
typedef struct {
  uint32_t sam : 1;
  uint32_t dam : 1;
  uint32_t syncdim : 1;
  uint32_t static_set : 1;
  uint32_t : 4;
  uint32_t fwid : 3;
  uint32_t tccmode : 1;
  uint32_t tcc : 6;
  uint32_t : 2;
  uint32_t tcinten : 1;
  uint32_t itcinten : 1;
  uint32_t tcchen : 1;
  uint32_t itcchen : 1;
  uint32_t privid : 4;
  uint32_t : 3;
  uint32_t priv : 1;
} edmaParamOpt;

typedef struct {
  uint32_t acnt : 16;
  uint32_t bcnt : 16;
} edmaParamABcnt;

typedef struct {
  uint32_t srcbidx : 16;
  uint32_t dstbidx : 16;
} edmaParamBidx;

typedef struct {
  uint32_t link : 16;
  uint32_t bcntrld : 16;
} edmaParamLnkRld;

typedef struct {
  uint32_t srccidx : 16;
  uint32_t dstcidx : 16;
} edmaParamCidx;

typedef struct {
  uint32_t ccnt : 16;
  uint32_t : 16;
} edmaParamCcnt;

typedef struct {
  edmaParamOpt    opt;
  uint32_t        src;
  edmaParamABcnt  abcnt;
  uint32_t        dst;
  edmaParamBidx   bidx;
  edmaParamLnkRld lnkrld;
  edmaParamCidx   cidx;
  edmaParamCcnt   ccnt;
} edmaParam;

#ifdef __cplusplus
}
#endif

#endif // LIB_DMA_AM335X_DMA_H
