#include "Resource.h"
#include "Transport_UDP_recv.h"

#define MAX_DATA_SIZE 65535

extern u_int8_t local_ip[4];
extern u_int8_t target_ip[4];

u_int8_t source_ip[4];
u_int16_t source_port;
char data_buffer[MAX_DATA_SIZE];

struct socket soc;
int sockid;

u_int16_t udp_check_sum(u_int8_t *udp_hdr, int len)
{
	int sum = 0, tmp = len;
	u_int16_t *p = (u_int16_t*)udp_hdr;
	while (len > 1)
	{
		sum += *p;
		len -= 2;
		p++;
	}

	//len=1 last one byte
	if (len)
	{
		sum += *((u_int8_t*)udp_hdr + tmp - 1);
	}

	//fold 32 bits to 16 bits
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return ~sum;
}

int sock(u_int8_t af, u_int8_t type, u_int8_t protocol)
{
	soc.af = af;
	soc.type = type;
	soc.protocol = protocol;
	return getpid();
}

void binds(struct sock_addr Servaddr)
{
	for (int i = 0; i < 4; i++)
	{
		Servaddr.ip[i] = local_ip[i];
	}
	Servaddr.port = htons(6000);

	sockid = sock(1, 17, IPPROTO_IP);
}

int recvfroms(struct sock_addr Servaddr, u_int8_t *udp_buffer, u_int16_t buflen, int flag, u_int8_t *suorsdd_ip, int adrlen)
{
	struct udp_fake_header *udp_fake_hdr = (struct udp_fake_header *)data_buffer;

	//填充伪首部
	for (int i = 0; i <= 4; i++)
	{
		udp_fake_hdr->dst_ip[i] = local_ip[i];
		udp_fake_hdr->sour_ip[i] = suorsdd_ip[i];
		source_ip[i] = suorsdd_ip[i];
	}
	udp_fake_hdr->zero = 0;
	udp_fake_hdr->type = 17;
	source_port = ((struct udp_header *)udp_buffer )->sour_port;
	udp_fake_hdr->udp_length = ((struct udp_header *)udp_buffer)->total_length ;

	int length = ntohs(((struct udp_header *)udp_buffer)->total_length)+12;

	for (int i = 12; i < length; i++)
	{
		data_buffer[i] = udp_buffer[i-12];
	}

	if (udp_check_sum((u_int8_t *)data_buffer, length) != 0)
	{
		printf("Error: UDP 校验和错误！!\n");
		//return 0;
	}

	struct udp_header *udp_hdr = (struct udp_header *)udp_buffer;
	if (udp_hdr->sour_port != source_port || udp_hdr->dst_port != Servaddr.port)
	{
		printf("UDP端口匹配错误!\n");
		//return 0;
	}
	int flag_ip = 0;
	for (int i = 0; i < 4; i++)
	{
		if (source_ip[i] != local_ip[i])
		{
			flag_ip = 1;
			
			break;
		}
	}
	if (flag_ip == 1)
	{
		printf("UDP包源IP与绑定socket源IP不一致\n");
		return 0;
	}

	//写入文件
	FILE *udp_file;
	udp_file = fopen("udp_recv.mp3", "ab+");
	udp_buffer = udp_buffer + 8;
	fwrite(udp_buffer, 1, length - 12 - 8, udp_file);
	if (0)
	{
		for (int i = 8; i < length - 12; i++)
		{
			fprintf(udp_file,"%c",data_buffer[i]);
		}
	}
	fclose(udp_file);
	printf("写入文件完成\n");
	return 0;

}