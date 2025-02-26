#include "xscugic.h"
#include "zynq_interrupt.h"
#include "init_dma.h"
#include "lwip_init.h"


#define INT_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID

XScuGic XScuGicInstance;

int lwip_loop();
int start_read_send = 0;
extern void init_platform();
extern void init_udp();
extern void cleanup_platform();

int main()
{
    init_platform();
    InterruptInit(INT_DEVICE_ID, &XScuGicInstance);
    init_dma_(&XScuGicInstance);
    init_udp();

    while (1)
    {
    	if (start_read_send)
    	{
    		lwip_loop();
			start_read_send = 0;
    	}
    }

    cleanup_platform();
    return 0;
}
