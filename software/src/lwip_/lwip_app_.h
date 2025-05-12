#ifndef LWIP_APP_H_
#define LWIP_APP_H_

#include "lwip/priv/tcp_priv.h"
#include "lwip/init.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xil_printf.h"
#include "lwip/udp.h"

#define F_ADC 200

// struct for configure
struct experiment_conf
{
    u16 Ng;
    u16 line_length;    
    u16 len_udp_pack;
    u16 freq_send_data;
    u16 pulse_width;
    u16 adc_byte_count;
};

extern struct experiment_conf current_conf;
extern u8 flag_start;

u32 compute_adc_points(int adc_freq);
void udp_recv_perf_traffic(void *arg, struct udp_pcb *tpcb,
		struct pbuf *p, const ip_addr_t *addr, u16_t port);
void print_new_config();
int send_adc_data(const char *frame, int data_len);

#endif