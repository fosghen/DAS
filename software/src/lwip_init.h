#ifndef ILIP_INIT_H_
#define ILIP_INIT_H_

#include "platform_config.h"
#include "xil_printf.h"
#include "zynq_interrupt.h"
#include "lwip/udp.h"
#include "adc_dma.h"

#define MAX_SEND_LEN ADC_BYTE*ADC_SAMPLE_NUM
#define F_ADC 50


void lwip_init();
void init_udp();
void print_app_header();
void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
void print_ip(char *msg, ip_addr_t *ip);
int start_udp(unsigned int port);
void udp_recive(void *arg, struct udp_pcb *pcb, struct pbuf *p_rx, const ip_addr_t *addr, u16_t port);
int transfer_data(const char *pData, int len, const ip_addr_t *addr);
int send_adc_data(const char *frame, int data_len);
uint32_t compute_adc_points(int n_g, int length, int adc_freq);

#endif /* ILIP_INIT_H_ */
