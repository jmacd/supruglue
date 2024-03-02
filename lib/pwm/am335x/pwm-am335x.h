// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/pwm/pwm-defs.h"

#ifndef LIB_PWM_AM335X_PWM_H
#define LIB_PWM_AM335X_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

// @@@ Belongs somewhere else, incomplete

#define EDMA_BASE ((volatile uint32_t *)(0x49000000))
#define EDMA_DRAE1 (0x348 / WORDSZ)  // DMA Region Access Enable Register for Region 1 Section 11.4.1.20
#define EDMA_DRAEH1 (0x34C / WORDSZ) // DMA Region Access Enable Register High for Region 1 Section 11.4.1.21

#define EDMAREG_IESR 0x60  // Interrupt Enable Set Register Section 11.4.1.113
#define EDMAREG_IESRH 0x64 // Interrupt Enable Set Register High Section 11.4.1.114

#define EDMAREG_SECR 0x40  // Secondary Event Clear Register Section 11.4.1.107
#define EDMAREG_SECRH 0x44 // Secondary Event Clear Register High Section 11.4.1.108

#define EDMAREG_ICR 0x70  // Interrupt Clear Register Section 11.4.1.117
#define EDMAREG_ICRH 0x74 // Interrupt Clear Register High Section 11.4.1.118

#define EDMAREG_EESR 0x30  // Event Enable Set Register Section 11.4.1.103
#define EDMAREG_EESRH 0x34 // Event Enable Set Register High Section 11.4.1.104

#define EDMA_EMCR (0x308 / WORDSZ)  // Event Missed Clear Register Section 11.4.1.11
#define EDMA_EMCRH (0x30C / WORDSZ) // Event Missed Clear Register High Section 11.4.1.12

#define EDMAREG_ER 0x00  // Event Register Section 11.4.1.91
#define EDMAREG_ERH 0x04 // Event Register High Section 11.4.1.92

#define EDMAREG_SER 0x38  // Secondary Event Register Section 11.4.1.105
#define EDMAREG_SERH 0x3C // Secondary Event Register High Section 11.4.1.106

#define EDMA_DCHMAP_N(N) ((0x100 + (N << 2)) / WORDSZ)
#define EDMA_DMAQNUM_4 (0x250 / WORDSZ) // DMA Queue Number Register 4
#define SHADOW1(reg) ((0x2200 + reg) / WORDSZ)

#define EDMA_dmaChannel 35
#define EDMA_dmaChannelMask (1 << (EDMA_dmaChannel - 32))
#define EDMA_paramNumber EDMA_dmaChannel
#define EDMA_paramNumberMask (1 << (EDMA_paramNumber - 32))

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

#endif // LIB_PWM_AM335X_PWM_H
