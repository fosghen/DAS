#include <stdio.h>
#include "lwip/priv/tcp_priv.h"
#include "platform.h"
#include "lwip_init_.h"
#include "lwip_app_.h"
#include "init_dma.h"


extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;

void platform_enable_interrupts(void);
void start_application(void);
void print_app_header(void);

u16 counter_refl = 0;
        
int main(void)
{
    init_platform();
    init_udp();
    XAxiDma_Initial(&AxiDma);
    RxDone = 1;
	while (1) {

		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
            
        }
		xemacif_input(&server_netif);

        if (RxDone & flag_start & flag_timer){
            RxDone = 0;
            flag_timer = 0;
            Xil_DCacheInvalidateRange((UINTPTR) DmaRxBuffer, current_conf.adc_byte_count);

            header[3] = (counter_refl >> 8) & 0xff;
			header[4] = (counter_refl) & 0xff;
            header[5] = (counter_refl >> 8) & 0xff;
		    header[6] = (counter_refl) & 0xff;
            send_adc_data((const char *) DmaRxBuffer, current_conf.adc_byte_count);
            // for (int i = 0; i < 250; i++){
            //     xil_printf("%d ", DmaRxBuffer[i]);
            // }
            // xil_printf("\r\n");  
            
            XAxiDma_Adc(AD9238_CH0_BASE, current_conf.adc_byte_count, current_conf.pulse_width, &AxiDma);
            counter_refl++;
        }
	}

	cleanup_platform();

	return 0;
}

