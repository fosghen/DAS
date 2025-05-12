#ifndef LWIP_INIT_H
#define LWIP_INIT_H

#include "lwip/priv/tcp_priv.h"
#include "lwip/init.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xil_printf.h"
#include "lwip/udp.h"

// plate's ips
#define DEFAULT_IP_ADDRESS	"192.168.1.10"
#define DEFAULT_IP_MASK		"255.255.255.0"
#define DEFAULT_GW_ADDRESS	"192.168.1.1"

// plate's mac address 
extern unsigned char mac_ethernet_address[];

// network interface of server
extern struct netif server_netif;
extern struct udp_pcb *pcb;

extern char header[8];

// seconds between periodic bandwidth reports
#define INTERIM_REPORT_INTERVAL 5
#define REPORT_INTERVAL_TIME (INTERIM_REPORT_INTERVAL * 20)

//server port to listen on/connect to
#define UDP_CONN_PORT 8080

int init_udp();
// void udp_recv_perf_traffic(void *arg, struct udp_pcb *tpcb,
// 		struct pbuf *p, const ip_addr_t *addr, u16_t port);

void start_application(void);

#endif // LWIP_INIT_H