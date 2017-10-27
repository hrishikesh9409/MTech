#pragma once 

#include <stdio.h>
#include <sys/types.h>

#define PCAP_ERRBUF_SIZE 256

typedef struct pcap pcap_t;

#ifdef MSDOS /* must be 32-bit */
typedef long          bpf_int32;
typedef unsigned long bpf_u_int32;
#else
typedef	int bpf_int32;
typedef	u_int bpf_u_int32;
#endif

struct bpf_insn {
	u_short	code;
	u_char 	jt;
	u_char 	jf;
	bpf_u_int32 k;
};

struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	bpf_u_int32 caplen;	/* length of portion present */
	bpf_u_int32 len;	/* length this packet (off wire) */
};

struct bpf_program {
	u_int bf_len;
	struct bpf_insn *bf_insns;
};

struct pcap_stat {
	u_int ps_recv;		/* number of packets received */
	u_int ps_drop;		/* number of packets dropped */
	u_int ps_ifdrop;	/* drops by interface -- only supported on some platforms */
#ifdef WIN32
	u_int bs_capt;		/* number of packets that reach the application */
#endif /* WIN32 */
};

struct pcap_sf {
	FILE *rfile;
	int swapped;
	int hdrsize;
	int version_major;
	int version_minor;
	u_char *base;
};


struct pcap_md {
	struct pcap_stat stat;
	/*XXX*/
	int use_bpf;		/* using kernel filter */
	u_long	TotPkts;	/* can't oflow for 79 hrs on ether */
	u_long	TotAccepted;	/* count accepted by filter */
	u_long	TotDrops;	/* count of dropped packets */
	long	TotMissed;	/* missed by i/f during this run */
	long	OrigMissed;	/* missed by i/f before this run */
#ifdef linux
	int	sock_packet;	/* using Linux 2.0 compatible interface */
	int	readlen;	/* byte count to hand to "recvmsg()" */
	int	timeout;	/* timeout specified to pcap_open_live */
	int	clear_promisc;	/* must clear promiscuous mode when we close */
	int	cooked;		/* using SOCK_DGRAM rather than SOCK_RAW */
	int	lo_ifindex;	/* interface index of the loopback device */
	char 	*device;	/* device name */
	struct pcap *next;	/* list of open promiscuous sock_packet pcaps */
#endif
};


struct pcap {
	int fd;
	int snapshot;
	int linktype;
	int tzoff;		/* timezone offset */
	int offset;		/* offset for proper alignment */

	struct pcap_sf sf;
	struct pcap_md md;

	/*
	 * Read buffer.
	 */
	int bufsize;
	u_char *buffer;
	u_char *bp;
	int cc;

	/*
	 * Place holder for pcap_next().
	 */
	u_char *pkt;

	
	/*
	 * Placeholder for filter code if bpf not in kernel.
	 */
	struct bpf_program fcode;

	char errbuf[PCAP_ERRBUF_SIZE];
};



pcap_t *
pcap_open_live(const char *device, int snaplen, int promisc, int to_ms, char *errbuf)
{
	pcap_t *p;
	int status;

	p = pcap_create(device, errbuf);
	if (p == NULL)
		return (NULL);
	status = pcap_set_snaplen(p, snaplen);
	if (status < 0)
		goto fail;
	status = pcap_set_promisc(p, promisc);
	if (status < 0)
		goto fail;
	status = pcap_set_timeout(p, to_ms);
	if (status < 0)
		goto fail;
	/*
	 * Mark this as opened with pcap_open_live(), so that, for
	 * example, we show the full list of DLT_ values, rather
	 * than just the ones that are compatible with capturing
	 * when not in monitor mode.  That allows existing applications
	 * to work the way they used to work, but allows new applications
	 * that know about the new open API to, for example, find out the
	 * DLT_ values that they can select without changing whether
	 * the adapter is in monitor mode or not.
	 */
	p->oldstyle = 1;
	status = pcap_activate(p);
	if (status < 0)
		goto fail;
	return (p);
fail:
	if (status == PCAP_ERROR)
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s", device,
		    p->errbu)f;
	else if (status == PCAP_ERROR_NO_SUCH_DEVICE ||
	    status == PCAP_ERROR_PERM_DENIED ||
	    status == PCAP_ERROR_PROMISC_PERM_DENIED)
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s (%s)", device,
		    pcap_statustostr(status), p->errbuf);
	else
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s", device,
		    pcap_statustostr(status));
	pcap_close(p);
	return (NULL);
}



int
pcap_compile(pcap_t *p, struct bpf_program *program,
	     const char *buf, int optimize, bpf_u_int32 mask)
{
	compiler_state_t cstate;
	const char * volatile xbuf = buf;
	yyscan_t scanner = NULL;
	YY_BUFFER_STATE in_buffer = NULL;
	u_int len;
	int  rc;

#ifdef _WIN32
	static int done = 0;

	if (!done)
		pcap_wsockinit();
	done = 1;
#endif

	/*
	 * If this pcap_t hasn't been activated, it doesn't have a
	 * link-layer type, so we can't use it.
	 */
	if (!p->activated) {
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
		    "not-yet-activated pcap_t passed to pcap_compile");
		rc = -1;
		goto quit;
	}
	initchunks(&cstate);
	cstate.no_optimize = 0;
	cstate.ai = NULL;
	cstate.ic.root = NULL;
	cstate.ic.cur_mark = 0;
	cstate.bpf_pcap = p;
	init_regs(&cstate);

	if (setjmp(cstate.top_ctx)) {
#ifdef INET6
		if (cstate.ai != NULL)
			freeaddrinfo(cstate.ai);
#endif
		rc = -1;
		goto quit;
	}

	cstate.netmask = mask;

	cstate.snaplen = pcap_snapshot(p);
	if (cstate.snaplen == 0) {
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
			 "snaplen of 0 rejects all packets");
		rc = -1;
		goto quit;
	}

	if (pcap_lex_init(&scanner) != 0)
		bpf_error(&cstate, "can't initialize scanner: %s", pcap_strerror(errno));
	in_buffer = pcap__scan_string(xbuf ? xbuf : "", scanner);

	/*
	 * Associate the compiler state with the lexical analyzer
	 * state.
	 */
	pcap_set_extra(&cstate, scanner);

	init_linktype(&cstate, p);
	(void)pcap_parse(scanner, &cstate);

	if (cstate.ic.root == NULL)
		cstate.ic.root = gen_retblk(&cstate, cstate.snaplen);

	if (optimize && !cstate.no_optimize) {
		bpf_optimize(&cstate, &cstate.ic);
		if (cstate.ic.root == NULL ||
		    (cstate.ic.root->s.code == (BPF_RET|BPF_K) && cstate.ic.root->s.k == 0))
			bpf_error(&cstate, "expression rejects all packets");
	}
	program->bf_insns = icode_to_fcode(&cstate, &cstate.ic, cstate.ic.root, &len);
	program->bf_len = len;

	rc = 0;  /* We're all okay */

quit:
	/*
	 * Clean up everything for the lexical analyzer.
	 */
	if (in_buffer != NULL)
		pcap__delete_buffer(in_buffer, scanner);
	if (scanner != NULL)
		pcap_lex_destroy(scanner);

	/*
	 * Clean up our own allocated memory.
	 */
	freechunks(&cstate);

	return (rc);
}

static int
snf_setfilter(pcap_t *p, struct bpf_program *fp)
{
	if (!p)
		return -1;
	if (!fp) {
		strncpy(p->errbuf, "setfilter: No filter specified",
			sizeof(p->errbuf));
		return -1;
	}

	/* Make our private copy of the filter */

	if (install_bpf_program(p, fp) < 0)
		return -1;

	return (0);
}

static void
initialize_ops(pcap_t *p)
{
	/*
	 * Set operation pointers for operations that only work on
	 * an activated pcap_t to point to a routine that returns
	 * a "this isn't activated" error.
	 */
	p->setfilter_op = (setfilter_op_t)pcap_not_initialized;
	p->cleanup_op = pcap_cleanup_live_common;

	/*
	 * In most cases, the standard one-shot callback can
	 * be used for pcap_next()/pcap_next_ex().
	 */
	p->oneshot_callback = pcap_oneshot;
}


static void
initialize_ops(pcap_t *p)
{
	/*
	 * Set operation pointers for operations that only work on
	 * an activated pcap_t to point to a routine that returns
	 * a "this isn't activated" error.
	 */
	p->read_op = (read_op_t)pcap_not_initialized;
	p->inject_op = (inject_op_t)pcap_not_initialized;
	p->setfilter_op = (setfilter_op_t)pcap_not_initialized;
	p->setdirection_op = (setdirection_op_t)pcap_not_initialized;
	p->set_datalink_op = (set_datalink_op_t)pcap_not_initialized;
	p->getnonblock_op = (getnonblock_op_t)pcap_not_initialized;
	p->setnonblock_op = (setnonblock_op_t)pcap_not_initialized;
	p->stats_op = (stats_op_t)pcap_not_initialized;
#ifdef _WIN32
	p->stats_ex_op = (stats_ex_op_t)pcap_not_initialized_ptr;
	p->setbuff_op = (setbuff_op_t)pcap_not_initialized;
	p->setmode_op = (setmode_op_t)pcap_not_initialized;
	p->setmintocopy_op = (setmintocopy_op_t)pcap_not_initialized;
	p->getevent_op = pcap_getevent_not_initialized;
	p->oid_get_request_op = (oid_get_request_op_t)pcap_not_initialized;
	p->oid_set_request_op = (oid_set_request_op_t)pcap_not_initialized;
	p->sendqueue_transmit_op = pcap_sendqueue_transmit_not_initialized;
	p->setuserbuffer_op = (setuserbuffer_op_t)pcap_not_initialized;
	p->live_dump_op = (live_dump_op_t)pcap_not_initialized;
	p->live_dump_ended_op = (live_dump_ended_op_t)pcap_not_initialized;
	p->get_airpcap_handle_op = pcap_get_airpcap_handle_not_initialized;
#endif

	/*
	 * Default cleanup operation - implementations can override
	 * this, but should call pcap_cleanup_live_common() after
	 * doing their own additional cleanup.
	 */
	p->cleanup_op = pcap_cleanup_live_common;

	/*
	 * In most cases, the standard one-shot callback can
	 * be used for pcap_next()/pcap_next_ex().
	 */
	p->oneshot_callback = pcap_oneshot;
}

int
pcap_setfilter(pcap_t *p, struct bpf_program *fp)
{
	return (p->setfilter_op(p, fp));
}