#include "lwip_init_.h"
#include "lwip_app_.h"
#include "platform.h"

// address PC
ip_addr_t target_addr;


struct experiment_conf current_conf = {0}; 
u8 flag_start;


void udp_recv_perf_traffic(void *arg, struct udp_pcb *tpcb,
		struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    xil_printf("start monolog\r\n");
		flag_start = 1;
		if (p){
			// 1000 * Ng
			current_conf.Ng = 0;
			current_conf.Ng |= (((char *)p->payload)[0] << 8);
			current_conf.Ng |= ((char *)p->payload)[1];
			// line length
			current_conf.line_length = 0;
			current_conf.line_length |= (((char *)p->payload)[2] << 8);
			current_conf.line_length |= ((char *)p->payload)[3];
			// length udp package
			current_conf.len_udp_pack = 0;
			current_conf.len_udp_pack |= (((char *)p->payload)[4] << 8);
			current_conf.len_udp_pack |= ((char *)p->payload)[5];
			// count reflectograms per sec
			current_conf.freq_send_data = 0;
			current_conf.freq_send_data |= (((char *)p->payload)[6] << 8);
			current_conf.freq_send_data |= ((char *)p->payload)[7];
			// pulse width in ns
			current_conf.pulse_width = 0;
			current_conf.pulse_width |= (((char *)p->payload)[8] << 8);
			current_conf.pulse_width |= ((char *)p->payload)[9];
			
            current_conf.adc_byte_count = compute_adc_points(F_ADC);
			update_timer(current_conf.freq_send_data);
            print_new_config();
		}
		target_addr.addr = addr->addr;
		pbuf_free(p);
        
}

u32 compute_adc_points(int adc_freq)
{
	return 2 * (2 * current_conf.Ng * current_conf.line_length * adc_freq) / (3 * 100000);
}

void print_new_config()
{
    xil_printf("Ng = %i\n\r", current_conf.Ng);
    xil_printf("Line length = %i\n\r", current_conf.line_length);
    xil_printf("Length udp package = %i\n\r", current_conf.len_udp_pack);
    xil_printf("Count reflectograms per sec = %i\n\r", current_conf.freq_send_data);
    xil_printf("Count bytes from adc = %i\n\r", current_conf.adc_byte_count);
    xil_printf("laser impulse = %i ns\n\r", current_conf.pulse_width);
}

int send_adc_data(const char *frame, int data_len)
{
	struct pbuf *q;
	q = pbuf_alloc(PBUF_TRANSPORT, 8 + data_len, PBUF_RAM);
	if (!q) {
		xil_printf("pbuf_alloc %d fail\n\r", data_len + 8);
		return -3;
	}

	memcpy(q->payload, header, 7);
	for (int i = 0; i < data_len; i += 2) {
		((char *) q->payload)[7 + i] = frame[i + 1];
		((char *) q->payload)[8 + i] = frame[i + 0];
	};
	q->len = q->tot_len = 7 + data_len;
	udp_sendto(pcb, q, &target_addr, UDP_CONN_PORT);
	pbuf_free(q);
	return 0;
}