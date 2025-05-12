/*
 * adc_dma.h
 *
 *  Created on: 2018��8��14��
 *      Author: Administrator
 */

#ifndef SRC_ADC_DMA_H_
#define SRC_ADC_DMA_H_

#include "xil_types.h"
#include "xstatus.h"

#define AD9238_SAMPLE_S00_AXI_SLV_REG0_OFFSET 0
#define AD9238_SAMPLE_S00_AXI_SLV_REG1_OFFSET 4
#define AD9238_SAMPLE_S00_AXI_SLV_REG2_OFFSET 8
#define AD9238_SAMPLE_S00_AXI_SLV_REG3_OFFSET 12

#define AD9238_SAMPLE_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

#define AD9238_SAMPLE_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))      

/*
 *DMA redefines
 */
#define MAX_DMA_LEN		   0x800000      /* DMA max length in byte */
#define CH0_DMA_DEV_ID	   XPAR_AXIDMA_0_DEVICE_ID
#define CH1_DMA_DEV_ID	   XPAR_AXIDMA_1_DEVICE_ID
#define CH0_DMA_BASE	   XPAR_AXIDMA_0_BASEADDR
#define CH1_DMA_BASE	   XPAR_AXIDMA_1_BASEADDR
#define CH0_S2MM_INTR_ID   XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR
#define CH1_S2MM_INTR_ID   XPAR_FABRIC_AXI_DMA_1_S2MM_INTROUT_INTR

/*
 *ADC defines
 */
#define AD9238_CH0_BASE    XPAR_AD9238_SAMPLE_0_BASEADDR
#define AD9238_CH1_BASE    XPAR_AD9238_SAMPLE_1_S00_AXI_BASEADDR
#define AD9238_START       AD9238_SAMPLE_S00_AXI_SLV_REG0_OFFSET
#define AD9238_LENGTH      AD9238_SAMPLE_S00_AXI_SLV_REG1_OFFSET
#define ADC_CAPTURELEN     1920           /* ADC capture length */
#define ADC_COE            16             /* ADC coefficient */
#define ADC_BYTE           2              /* ADC data byte number */
#define ADC_BITS           12


/*
 *DMA BD defines
 */
#define BD_COUNT         4


#define ADC_SAMPLE_NUM  (1024*512)

#endif /* SRC_ADC_DMA_H_ */
