#include "init_dma.h"
#include "adc_dma.h"


// flag dma
int RxDone = 0;
// dma buffer
unsigned short DmaRxBuffer[MAX_DMA_LEN/ADC_BYTE]  __attribute__ ((aligned(64)));
// instance dma
XAxiDma AxiDma;

int XAxiDma_Initial(XAxiDma *AxiDma)
{
	XAxiDma_Config *Config;
	int Status;
	
    Config = XAxiDma_LookupConfig(XPAR_XAXIDMA_0_BASEADDR);
	if (!Config) {
		xil_printf("No config found for %d\r\n", XPAR_XAXIDMA_0_BASEADDR);

		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(AxiDma, Config);

	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(AxiDma)) {
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

    XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

	Status = XSetupInterruptSystem(AxiDma, &RxIntrHandler,
				       Config->IntrId[0], Config->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS ;
}

void RxIntrHandler(void *Callback)
{
	u32 IrqStatus;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DEVICE_TO_DMA);
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}
    
    if (IrqStatus & XAXIDMA_IRQ_ERROR_MASK) {
    	RxDone = 1;
        XAxiDma_Reset(AxiDmaInst);
        while (!XAxiDma_ResetIsDone(AxiDmaInst));
        return;
    }
    
    if (IrqStatus & XAXIDMA_IRQ_IOC_MASK) {
    	RxDone = 1;
        // xil_printf("dma handler");
    }
}

void XAxiDma_Adc(u32 adc_addr, u32 adc_len, u16 pulse_width, XAxiDma *AxiDma)
{
    int Status;
	ad9238_sample(adc_addr, adc_len / 2, pulse_width);
    Status = XAxiDma_SimpleTransfer(AxiDma, (UINTPTR) DmaRxBuffer,
                                    adc_len, XAXIDMA_DEVICE_TO_DMA);

    if (Status != XST_SUCCESS) {
        xil_printf("Error: RX transfer start failed with status %d\r\n", Status);
        return;
    }

}


void ad9238_sample(u32 adc_addr, u32 adc_len, u16 pulse_width)
{
	/* provide length to AD9238 channel */
	AD9238_SAMPLE_mWriteReg(adc_addr, AD9238_LENGTH, adc_len)  ;
	AD9238_SAMPLE_mWriteReg(adc_addr, 8, pulse_width)  ;
    AD9238_SAMPLE_mWriteReg(adc_addr, 12, 0)  ;
	/* start sample AD9238 channel */
	AD9238_SAMPLE_mWriteReg(adc_addr, AD9238_START, 1) ;
}