#ifndef ILIP_INIT_H_
#define ILIP_INIT_H_

#include "xparameters.h"
#include "xaxidma.h"

int XAxiDma_Initial(u16 DeviceId, u16 IntrID, XAxiDma *XAxiDma, XScuGic *InstancePtr);
void init_dma_(XScuGic *XScuGicInstance);
void Dma_Interrupt_Handler(void *CallBackRef);
void XAxiDma_Adc(u32 *BdChainBuffer, u32 adc_addr, u32 adc_len, u16 pulse_width, u16 BdCount, XAxiDma *AxiDma);
void ad9238_sample(u32 adc_addr, u32 adc_len, u16 pulse_width);

#endif
