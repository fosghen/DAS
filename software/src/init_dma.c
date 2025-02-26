#include "zynq_interrupt.h"
#include "dma_bd/dma_bd.h"
#include "init_dma.h"
#include "xaxidma.h"
#include "xscugic.h"
#include "adc_dma.h"

// очищаем флаги DMA
int ch0_s2mm_flag = -1 ;
//int ch1_s2mm_flag = -1 ;
// буффер для данных из DMA
short CH0DmaRxBuffer[MAX_DMA_LEN/ADC_BYTE]  __attribute__ ((aligned(64)));
//short CH1DmaRxBuffer[MAX_DMA_LEN/ADC_BYTE]  __attribute__ ((aligned(64)));
// вот это не знаю что
u32 Ch0BdChainBuffer[BD_ALIGNMENT*16] __attribute__ ((aligned(64)));
//u32 Ch1BdChainBuffer[BD_ALIGNMENT*16] __attribute__ ((aligned(64)));
// стуктура DMA
XAxiDma AxiDmaCh0;
XAxiDma AxiDmaCh1;


int XAxiDma_Initial(u16 DeviceId, u16 IntrID, XAxiDma *XAxiDma, XScuGic *InstancePtr)
{
	XAxiDma_Config *CfgPtr;
	int Status;
	/* Initialize the XAxiDma device. */
	CfgPtr = XAxiDma_LookupConfig(DeviceId);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(XAxiDma, CfgPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}


	/* Disable MM2S interrupt, Enable S2MM interrupt */
	XAxiDma_IntrEnable(XAxiDma, XAXIDMA_IRQ_IOC_MASK,
			XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(XAxiDma, XAXIDMA_IRQ_ALL_MASK,
			XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS ;
}

void Dma_Interrupt_Handler(void *CallBackRef)
{
	XAxiDma *XAxiDmaPtr ;
	XAxiDmaPtr = (XAxiDma *) CallBackRef ;

	int s2mm_sr ;

	s2mm_sr = XAxiDma_IntrGetIrq(XAxiDmaPtr, XAXIDMA_DEVICE_TO_DMA) ;
	//xil_printf("Interrupt Value is 0x%x\r\n", s2mm_sr) ;

	if (s2mm_sr & XAXIDMA_IRQ_IOC_MASK)
	{
		/* Clear interrupt */
		XAxiDma_IntrAckIrq(XAxiDmaPtr, XAXIDMA_IRQ_IOC_MASK,
				XAXIDMA_DEVICE_TO_DMA) ;

		if (XAxiDmaPtr->RegBase == CH0_DMA_BASE)
		{
			ch0_s2mm_flag = 1;
		}
//
//		else if (XAxiDmaPtr->RegBase == CH1_DMA_BASE)
//		{
//			ch1_s2mm_flag = 1;
//		}
	}
}

void init_dma_(XScuGic *XScuGicInstance)
{
	/* Initialize DMA */
	XAxiDma_Initial(CH0_DMA_DEV_ID, CH0_S2MM_INTR_ID, &AxiDmaCh0, XScuGicInstance) ;
//	XAxiDma_Initial(CH1_DMA_DEV_ID, CH1_S2MM_INTR_ID, &AxiDmaCh1, XScuGicInstance) ;
	/* Interrupt register */
	InterruptConnect(XScuGicInstance,CH0_S2MM_INTR_ID,Dma_Interrupt_Handler, &AxiDmaCh0,0,3);
//	InterruptConnect(XScuGicInstance,CH1_S2MM_INTR_ID,Dma_Interrupt_Handler, &AxiDmaCh1,0,3);
	/* Create BD chain */
	CreateBdChain(Ch0BdChainBuffer, BD_COUNT, ADC_BYTE*ADC_SAMPLE_NUM, (u8 *)CH0DmaRxBuffer, RXPATH) ;
//	CreateBdChain(Ch1BdChainBuffer, BD_COUNT, ADC_BYTE*ADC_SAMPLE_NUM, (u8 *)CH1DmaRxBuffer, RXPATH) ;
}

void XAxiDma_Adc(u32 *BdChainBuffer, u32 adc_addr, u32 adc_len, u16 pulse_width, u16 BdCount, XAxiDma *AxiDma)
{
	/* Clear BD Status */
	Bd_StatusClr(BdChainBuffer, BdCount) ;
	/* Start Channel 0 sample */
	ad9238_sample(adc_addr, adc_len, pulse_width)  ;
	/* start DMA translation from AD9238 channel 0 to DDR3 */
	Bd_Start(BdChainBuffer, BdCount, AxiDma, RXPATH);
}

/*
 *callback function
 *Check interrupt status and assert s2mm flag
 */



/*
 *This is ADC sample function, use it and start sample adc data
 *
 *@param adc_addr ADC base address
 *@param adc_len is sample length in ADC data width
 */
void ad9238_sample(u32 adc_addr, u32 adc_len, u16 pulse_width)
{
	/* provide length to AD9238 channel */
	AD9238_SAMPLE_mWriteReg(adc_addr, AD9238_LENGTH, adc_len)  ;
	AD9238_SAMPLE_mWriteReg(adc_addr, 8, pulse_width)  ;
	/* start sample AD9238 channel */
	AD9238_SAMPLE_mWriteReg(adc_addr, AD9238_START, 1) ;
}
