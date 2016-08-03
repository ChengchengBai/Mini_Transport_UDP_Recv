#include "ARP_Cache_Table.h"
#include "Network_ARP.h "
#include "Resource.h"
#include "Ethernet.h"
#include "Network_IPV4.h"
#include "Header_Include.h"
#include "Network_ICMP.h"
#include "Transport_UDP_recv.h"


pcap_t *handle;

int main()
{
	//initial the arp_table
	init_arp_table();
	//output_arp_table();

	open_device();

	pcap_loop(handle, NULL, ethernet_protocol_packet_callback, NULL);


	close_device();
	return 0;
}