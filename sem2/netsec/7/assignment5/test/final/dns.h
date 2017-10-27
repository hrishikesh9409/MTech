#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <netinet/if_ether.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <resolv.h>

#define IP_SIZE 16
#define REQUEST_SIZE 100
#define FILTER_SIZE 200
#define DATAGRAM_SIZE 8192

char ORIGINAL_SITE[20];
char SPOOF_IP[20];


/* ethernet header definition */
struct etherHEADER{
	unsigned char ether_dhost[ETHER_ADDR_LEN]; /* dst address */
	unsigned char ether_shost[ETHER_ADDR_LEN]; /* src address */
	unsigned short ether_type; /* network protocol */
};

/* DNS query structure */
struct DNS_query {
	char *query_name;
	char query_type[2];
	char query_class[2];
};

/* DNS header definition */
struct dns_HEADER {
	char ID[2];
	char flags[2];
	char qd_count[2];
	char an_count[2];
	char ns_count[2];
	char ar_count[2];
};

/*	FUNCTION PROTOTYPES	*/
void print_message(char* request, char* ip);
unsigned short check_sum(unsigned short *buf, int nwords);
void extract_ip_from_iphdr(u_int32_t raw_ip, char* ip);
unsigned int build_dns_answer(char *SPOOF_IP, struct dns_HEADER *dns_hdr, char* answer, char* request);
void build_udp_ip_datagram(char* datagram, unsigned int payload_size, char* src_ip, char* dst_ip, u_int16_t port);
void extract_dns_request(struct DNS_query *dns_query, char *request);
void extract_port_from_udphdr(struct udphdr* udp, u_int16_t* port);
void extract_dns_data(const unsigned char *packet, struct dns_HEADER **dns_hdr, struct DNS_query *dns_query, char* src_ip, char* dst_ip, u_int16_t *port);
void send_dns_answer(char* ip, u_int16_t port, char* packet, int packlen);
void DNS_packet_handler(const unsigned char *packet);
void verify_DNS_packet(unsigned char *Buffer , int Size);
void process_packet(unsigned char* buffer, int size);
