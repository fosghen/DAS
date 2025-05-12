#include "lwip_init_.h"
#include "lwip_app_.h"

struct netif server_netif;
struct udp_pcb *pcb;

unsigned char mac_ethernet_address[] = {
		0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

char header[8] = {0};

int init_udp()
{
    struct netif *netif = &server_netif;
    struct ip4_addr ipaddr, netmask, gw;

	lwip_init();

    ip4addr_aton(DEFAULT_IP_ADDRESS, &ipaddr);
	ip4addr_aton(DEFAULT_IP_MASK, &netmask);
	ip4addr_aton(DEFAULT_GW_ADDRESS, &gw);

	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address,
	                PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\r\n");
		return -1;
	}
	netif_set_default(netif);
	netif_set_up(netif);

	start_application();
}

void start_application(void)
{
	err_t err;

	pcb = udp_new();
	if (!pcb) {
		xil_printf("UDP server: Error creating PCB. Out of Memory\r\n");
		return;
	}

	err = udp_bind(pcb, IP_ADDR_ANY, UDP_CONN_PORT);
	if (err != ERR_OK) {
		xil_printf("UDP server: Unable to bind to port");
		xil_printf(" %d: err = %d\r\n", UDP_CONN_PORT, err);
		udp_remove(pcb);
		return;
	}

	udp_recv(pcb, udp_recv_perf_traffic, NULL);

	return;
}
