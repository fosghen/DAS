#include "netif/xadapter.h"
#include "lwip_init.h"

static struct netif server_netif;
struct netif *echo_netif;

ip_addr_t ipaddr, netmask, gw;

unsigned char ip_export[4];
unsigned char mac_export[6];

struct udp_pcb *udp8080_pcb = NULL;
struct pbuf *udp8080_q = NULL;
int udp8080_qlen = 0;

u32 adc_sample_num;
int Ng;
int line_length;
int len_udp_pack;
int freq_send_data;
int pulse_width;

ip_addr_t target_addr;

char TargetHeader[8] = { 0, 0x00, 0x01, 0x00, 0x02 };
int FrameLengthCurr = 0;
int flag_start = 0;

void init_platform();
void platform_enable_interrupts();
void update_timer(int freq);

void init_udp()
{
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	echo_netif = &server_netif;
	IP4_ADDR(&ipaddr,  192, 168,   1, 10);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      192, 168,   1,  1);

	print_app_header();

	lwip_init();

	if (!xemac_add(echo_netif, &ipaddr, &netmask,
			&gw, mac_ethernet_address,
			PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return;
	}

	netif_set_default(echo_netif);

	/* now enable interrupts */
	init_platform();
	platform_enable_interrupts();

	/* specify that the network if is up */
	netif_set_up(echo_netif);

	IP4_ADDR(&target_addr, 192, 168, 1, 42);
	print_ip_settings(&ipaddr, &netmask, &gw);
	memcpy(ip_export, &ipaddr, 4);
	memcpy(mac_export, &mac_ethernet_address, 6);

	/* start the application (web server, rxtest, txtest, etc..) */
	start_udp(8080);
}

void print_app_header() {
	xil_printf("UDP packets sent to port 8080\n\r");
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

void print_ip(char *msg, ip_addr_t *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

int start_udp(unsigned int port) {
	err_t err;
	udp8080_pcb = udp_new();
	if (!udp8080_pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}
	/* bind to specified @port */
	err = udp_bind(udp8080_pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}
	udp_recv(udp8080_pcb, udp_recive, 0);
	IP4_ADDR(&target_addr, 192,168,1,42);

	return 0;
}

void udp_recive(void *arg, struct udp_pcb *pcb, struct pbuf *p_rx,
		const ip_addr_t *addr, u16_t port) {
		xil_printf("start monolog\r\n");
		flag_start = 1;
		if (p_rx){

			// 1000 * Ng
			Ng = 0;
			Ng |= (((char *)p_rx->payload)[0] << 8);
			Ng |= ((char *)p_rx->payload)[1];
			// line length
			line_length = 0;
			line_length |= (((char *)p_rx->payload)[2] << 8);
			line_length |= ((char *)p_rx->payload)[3];
			// length udp package
			len_udp_pack = 0;
			len_udp_pack |= (((char *)p_rx->payload)[4] << 8);
			len_udp_pack |= ((char *)p_rx->payload)[5];
			// count reflectograms per sec
			freq_send_data = 0;
			freq_send_data |= (((char *)p_rx->payload)[6] << 8);
			freq_send_data |= ((char *)p_rx->payload)[7];
			// pulse width in ns
			pulse_width = 0;
			pulse_width |= (((char *)p_rx->payload)[8] << 8);
			pulse_width |= ((char *)p_rx->payload)[9];
			adc_sample_num = compute_adc_points(Ng, line_length, F_ADC);
			update_timer(freq_send_data);

			xil_printf("Ng = %i\n\r", Ng);
			xil_printf("Line length = %i\n\r", line_length);
			xil_printf("Length udp package = %i\n\r", len_udp_pack);
			xil_printf("Count reflectograms per sec = %i\n\r", freq_send_data);
			xil_printf("Count bytes from adc = %i\n\r", adc_sample_num);
			xil_printf("laser impulse = %i ns\n\r", pulse_width);

		}
		target_addr.addr = addr->addr;
		pbuf_free(p_rx);
}


int send_adc_data(const char *frame, int data_len)
{
	struct pbuf *q;
	q = pbuf_alloc(PBUF_TRANSPORT, 8 + data_len, PBUF_RAM);
	if (!q) {
		xil_printf("pbuf_alloc %d fail\n\r", data_len + 8);
		return -3;
	}

	memcpy(q->payload, TargetHeader, 5);
	for (int i = 0; i < data_len; i += 2) {
		((char *) q->payload)[5 + i] = frame[i + 1];
		((char *) q->payload)[6 + i] = frame[i + 0];
	};
	q->len = q->tot_len = 5 + data_len;
	udp_sendto(udp8080_pcb, q, &target_addr, 8080);
	pbuf_free(q);
	return 0;
}



uint32_t compute_adc_points(int n_g, int length, int adc_freq) {
    return (uint32_t)(
        ((u64)2 * ((u64)2 * n_g * length * adc_freq)) / ((u64)3 * 100000)
    );
}
