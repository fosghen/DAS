#include <stdio.h>

#include "xparameters.h"

#include "netif/xadapter.h"
#include "lwip/udp.h"
#include "platform.h"
#include "platform_config.h"
#include "xil_printf.h"

#include "xaxidma.h"
#include "adc_dma.h"
#include "sleep.h"
#include "dma_bd/dma_bd.h"

#include "xil_cache.h"
#include "lwip_init.h"
#include "xscutimer.h"

extern char TargetHeader[8];
extern XAxiDma AxiDmaCh0;
extern short CH0DmaRxBuffer[MAX_DMA_LEN/ADC_BYTE] ;
extern u32 Ch0BdChainBuffer[BD_ALIGNMENT*16] ;
extern int ch0_s2mm_flag ;
extern struct netif *echo_netif;
extern int flag_start;
extern int adc_sample_num;
extern int len_udp_pack;
extern int pulse_width;

extern void XAxiDma_Adc(u32 *BdChainBuffer, u32 adc_addr, u32 adc_len, u16 pulse_width, u16 BdCount, XAxiDma *AxiDma);

u8 first_launch = 1;

int lwip_loop()
{
	// запуск работы lwip
	xemacif_input(echo_netif);

	if (flag_start)
	{
		if (first_launch)
		{
			// запрос данных с АЦП
			XAxiDma_Adc(Ch0BdChainBuffer, AD9238_CH0_BASE, adc_sample_num, pulse_width, BD_COUNT, &AxiDmaCh0) ;
			first_launch = 0;
		}
		// ожидание данных из DMA (около 150 мкс), можно добаить таймер
		while (ch0_s2mm_flag == -1) usleep(1);

		int udp_len;
		// очищаем кэш нужного размера
		Xil_DCacheInvalidateRange((u32) CH0DmaRxBuffer, adc_sample_num);

		// оправляем по пакетам
		for (int i = 0; i < adc_sample_num; i += len_udp_pack)
		{
			if ((i + len_udp_pack) > adc_sample_num)
				udp_len = adc_sample_num - i;
			else
				udp_len = len_udp_pack;
			// номер пакета
			TargetHeader[1] = (i / len_udp_pack >> 8) & 0xff;
			TargetHeader[2] = (i / len_udp_pack) & 0xff;
			// опправка данных
			send_adc_data((const char *) CH0DmaRxBuffer + i, udp_len);
		}
		XAxiDma_Adc(Ch0BdChainBuffer, AD9238_CH0_BASE, adc_sample_num, pulse_width, BD_COUNT, &AxiDmaCh0) ;
		ch0_s2mm_flag = -1;
	}
	return 0;
}






