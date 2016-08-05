#include "Header_Include.h"
#include <process.h>

struct udp_header
{
	u_int16_t sour_port;
	u_int16_t dst_port;
	u_int16_t total_length;
	u_int16_t check_sum;
};

struct  udp_fake_header
{
	u_int16_t sour_ip[4];
	u_int16_t dst_ip[4];
	u_int8_t zero;
	u_int8_t type;
	u_int16_t udp_length;

};

struct socket
{
	u_int8_t af;
	u_int8_t type;
	u_int8_t protocol;
};

struct  sock_addr
{
	u_int16_t port;
	u_int8_t ip[4];
};


int recvfroms(struct sock_addr Servaddr, u_int8_t *udp_buffer, u_int16_t buflen, int flag, u_int8_t *suorsdd_ip, int adrlen);
void binds(struct sock_addr Servaddr);
u_int16_t udp_check_sum(u_int8_t *udp_hdr, int len);
int sock(u_int8_t af, u_int8_t type, u_int8_t protocol);