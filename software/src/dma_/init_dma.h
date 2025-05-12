#ifndef INIT_DMA_H_
#define INIT_DMA_H_

#include "xaxidma.h"
#include "adc_dma.h"
#include "xparameters.h"
#include "xaxidma.h"
#include "xinterrupt_wrap.h"

extern unsigned short DmaRxBuffer[MAX_DMA_LEN/ADC_BYTE]  __attribute__ ((aligned(32)));
extern int RxDone;
extern XAxiDma AxiDma;

int XAxiDma_Initial(XAxiDma *AxiDma);
void RxIntrHandler(void *Callback);
void XAxiDma_Adc(u32 adc_addr, u32 adc_len, u16 pulse_width, XAxiDma *AxiDma);
void ad9238_sample(u32 adc_addr, u32 adc_len, u16 pulse_width);

#endif
