#pragma once 

#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <wchar.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#ifdef MSDOS /* must be 32-bit */
typedef long          bpf_int32;
typedef unsigned long bpf_u_int32;
#else
typedef	int bpf_int32;
typedef	u_int bpf_u_int32;
#endif

struct pcap_opt {
	char	*device;
	int	timeout;	/* timeout for buffering */
	u_int	buffer_size;
	int	promisc;
	int	rfmon;		/* monitor mode */
	int	immediate;	/* immediate mode - deliver packets as soon as they arrive */
	int	tstamp_type;
	int	tstamp_precision;
};

struct pcap_opt opt;

static struct capture_source_type {
	int (*findalldevs_op)(pcap_if_t **, char *);
	pcap_t *(*create_op)(const char *, char *, int *);
} capture_source_types[] = {
#ifdef HAVE_DAG_API
	{ dag_findalldevs, dag_create },
#endif
#ifdef HAVE_SEPTEL_API
	{ septel_findalldevs, septel_create },
#endif
#ifdef HAVE_SNF_API
	{ snf_findalldevs, snf_create },
#endif
#ifdef HAVE_TC_API
	{ TcFindAllDevs, TcCreate },
#endif
#ifdef PCAP_SUPPORT_BT
	{ bt_findalldevs, bt_create },
#endif
#ifdef PCAP_SUPPORT_BT_MONITOR
	{ bt_monitor_findalldevs, bt_monitor_create },
#endif
#ifdef PCAP_SUPPORT_USB
	{ usb_findalldevs, usb_create },
#endif
#ifdef PCAP_SUPPORT_NETFILTER
	{ netfilter_findalldevs, netfilter_create },
#endif
#ifdef PCAP_SUPPORT_DBUS
	{ dbus_findalldevs, dbus_create },
#endif
	{ NULL, NULL }
};



const char *
pcap_strerror(int errnum)
{
#ifdef HAVE_STRERROR
#ifdef _WIN32
	static char errbuf[PCAP_ERRBUF_SIZE];
	errno_t errno;
	errno = strerror_s(errbuf, PCAP_ERRBUF_SIZE, errnum);
	if (errno != 0) /* errno = 0 if successful */
		strlcpy(errbuf, "strerror_s() error", PCAP_ERRBUF_SIZE);
	return (errbuf);
#else
	return (strerror(errnum));
#endif /* _WIN32 */
#else
	extern int sys_nerr;
	extern const char *const sys_errlist[];
	static char errbuf[PCAP_ERRBUF_SIZE];

	if ((unsigned int)errnum < sys_nerr)
		return ((char *)sys_errlist[errnum]);
	(void)pcap_snprintf(errbuf, sizeof errbuf, "Unknown error: %d", errnum);
	return (errbuf);
#endif
}

#ifndef HAVE_VSNPRINTF
static int
sn_reserve (struct state *state, size_t n)
{
  return state->s + n > state->theend;
}

static int
sn_append_char (struct state *state, unsigned char c)
{
  if (sn_reserve (state, 1)) {
    return 1;
  } else {
    *state->s++ = c;
    return 0;
  }
}
#endif

static int
append_char(struct state *state,
	    unsigned char arg,
	    int width,
	    int flags)
{
  while(!(flags & minus_flag) && --width > 0)
    if((*state->append_char) (state, ' '))
      return 1;

  if((*state->append_char) (state, arg))
    return 1;
  while((flags & minus_flag) && --width > 0)
    if((*state->append_char) (state, ' '))
      return 1;

  return 0;
}

struct state {
  unsigned char *str;
  unsigned char *s;
  unsigned char *theend;
  size_t sz;
  size_t max_sz;
  int (*append_char)(struct state *, unsigned char);
  int (*reserve)(struct state *, size_t);
  /* XXX - methods */
};

#ifndef HAVE_VSNPRINTF
int
pcap_vsnprintf (char *str, size_t sz, const char *format, va_list args)
{
  struct state state;
  int ret;
  unsigned char *ustr = (unsigned char *)str;

  state.max_sz = 0;
  state.sz     = sz;
  state.str    = ustr;
  state.s      = ustr;
  state.theend = ustr + sz - 1;
  state.append_char = sn_append_char;
  state.reserve     = sn_reserve;

  ret = xyzprintf (&state, format, args);
  *state.s = '\0';
  if (ret)
    return sz;
  else
    return state.s - state.str;
}
#endif


#ifndef HAVE_SNPRINTF
int
pcap_snprintf (char *str, size_t sz, const char *format, ...)
{
  va_list args;
  int ret;

  va_start(args, format);
  ret = pcap_vsnprintf (str, sz, format, args);

#ifdef PARANOIA
  {
    int ret2;
    char *tmp;

    tmp = malloc (sz);
    if (tmp == NULL)
      abort ();

    ret2 = pcap_vsprintf (tmp, format, args);
    if (ret != ret2 || strcmp(str, tmp))
      abort ();
    free (tmp);
  }
#endif

  va_end(args);
  return ret;
}
#endif


pcap_t *
pcap_create(const char *device, char *errbuf)
{
	size_t i;
	int is_theirs;
	pcap_t *p;
	char *device_str;

	/*
	 * A null device name is equivalent to the "any" device -
	 * which might not be supported on this platform, but
	 * this means that you'll get a "not supported" error
	 * rather than, say, a crash when we try to dereference
	 * the null pointer.
	 */
	if (device == NULL)
		device_str = strdup("any");
	else {
#ifdef _WIN32
		/*
		 * If the string appears to be little-endian UCS-2/UTF-16,
		 * convert it to ASCII.
		 *
		 * XXX - to UTF-8 instead?  Or report an error if any
		 * character isn't ASCII?
		 */
		if (device[0] != '\0' && device[1] == '\0') {
			size_t length;

			length = wcslen((wchar_t *)device);
			device_str = (char *)malloc(length + 1);
			if (device_str == NULL) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "malloc: %s", pcap_strerror(errno));
				return (NULL);
			}

			pcap_snprintf(device_str, length + 1, "%ws",
			    (const wchar_t *)device);
		} else
#endif
			device_str = strdup(device);
	}
	if (device_str == NULL) {
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "malloc: %s", pcap_strerror(errno));
		return (NULL);
	}

	/*
	 * Try each of the non-local-network-interface capture
	 * source types until we find one that works for this
	 * device or run out of types.
	 */
	for (i = 0; capture_source_types[i].create_op != NULL; i++) {
		is_theirs = 0;
		p = capture_source_types[i].create_op(device_str, errbuf,
		    &is_theirs);
		if (is_theirs) {
			/*
			 * The device name refers to a device of the
			 * type in question; either it succeeded,
			 * in which case p refers to a pcap_t to
			 * later activate for the device, or it
			 * failed, in which case p is null and we
			 * should return that to report the failure
			 * to create.
			 */
			if (p == NULL) {
				/*
				 * We assume the caller filled in errbuf.
				 */
				free(device_str);
				return (NULL);
			}
			p->opt.device = device_str;
			return (p);
		}
	}

	/*
	 * OK, try it as a regular network interface.
	 */
	p = pcap_create_interface(device_str, errbuf);
	if (p == NULL) {
		/*
		 * We assume the caller filled in errbuf.
		 */
		free(device_str);
		return (NULL);
	}
	p->opt.device = device_str;
	return (p);
}

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
        p->errbuf);
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



struct pcap_stat {
	u_int ps_recv;		/* number of packets received */
	u_int ps_drop;		/* number of packets dropped */
	u_int ps_ifdrop;	/* drops by interface -- only supported on some platforms */
#ifdef WIN32
	u_int bs_capt;		/* number of packets that reach the application */
#endif /* WIN32 */
};

struct bpf_insn {
	u_short	code;
	u_char 	jt;
	u_char 	jf;
	bpf_u_int32 k;
};

struct bpf_program {
	u_int bf_len;
	struct bpf_insn *bf_insns;
};


typedef enum {
  	NOT_SWAPPED,
  	SWAPPED,
	MAYBE_SWAPPED
} swapped_type_t;

struct pcap_sf {
	FILE *rfile;
	int swapped;
	int hdrsize;
	swapped_type_t lengths_swapped;
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
	int	timeout;	/* timeout specified to pcap_open_live */
	int	clear_promisc;	/* must clear promiscuous mode when we close */
	int	cooked;		/* using SOCK_DGRAM rather than SOCK_RAW */
	int	lo_ifindex;	/* interface index of the loopback device */
	char	*device;	/* device name */
	struct pcap *next;	/* list of open promiscuous sock_packet pcaps */
#endif

#ifdef HAVE_DAG_API
	void	*dag_mem_base;	/* DAG card memory base address */
	u_int	dag_mem_bottom;	/* DAG card current memory bottom pointer */
	u_int	dag_mem_top;	/* DAG card current memory top pointer */
	int	dag_fcs_bits;	/* Number of checksum bits from link layer */
	int dag_offset_flags; /* Flags to pass to dag_offset(). */
#endif

#ifdef HAVE_REMOTE
/*!
	There is really a mess with previous variables, and it seems to me that they are not used
	(they are used in pcap_pf.c only). I think we have to start using them.
	The meaning is the following:

	- TotPkts: the amount of packets received by the bpf filter, *before* applying the filter
	- TotAccepted: the amount of packets that satisfies the filter
	- TotDrops: the amount of packet that were dropped into the kernel buffer because of lack of space
	- TotMissed: the amount of packets that were dropped by the physical interface; it is basically 
	the value of the hardware counter into the card. This number is never put to zero, so this number
	takes into account the *total* number of interface drops starting from the interface power-on.
	- OrigMissed: the amount of packets that were dropped by the interface *when the capture begins*.
	This value is used to detect the number of packets dropped by the interface *during the present
	capture*, so that (ps_ifdrops= TotMissed - OrigMissed).
*/
	unsigned int TotNetDrops;	//!< keeps the number of packets that have been dropped by the network
/*!
	\brief It keeps the number of packets that have been received by the application.
	
	Packets dropped by the kernel buffer are not counted in this variable. The variable is always 
	equal to (TotAccepted - TotDrops), exept for the case of remote capture, in which we have also
	packets in fligh, i.e. that have been transmitted by the remote host, but that have not been 
	received (yet) from the client. In this case, (TotAccepted - TotDrops - TotNetDrops) gives a
	wrong result, since this number does not corresponds always to the number of packet received by 
	the application. For this reason, in the remote capture we need another variable that takes
	into account of the number of packets actually received by the application.
*/
	unsigned int TotCapt;
#endif /* HAVE_REMOTE */
};

struct pcap {
#ifdef WIN32
	ADAPTER *adapter;
	LPPACKET Packet;
	int timeout;
	int nonblock;
#else
	int fd;
	int selectable_fd;
#endif /* WIN32 */
	int snapshot;
	int linktype;
	int tzoff;		/* timezone offset */
	int offset;		/* offset for proper alignment */

	int break_loop;		/* flag set to force break from packet-reading loop */

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
	 * Methods.
	 */
	int	(*read_op)(pcap_t *, int cnt, pcap_handler, u_char *);
	int	(*setfilter_op)(pcap_t *, struct bpf_program *);
	int	(*set_datalink_op)(pcap_t *, int);
	int	(*getnonblock_op)(pcap_t *, char *);
	int	(*setnonblock_op)(pcap_t *, int, char *);
	int	(*stats_op)(pcap_t *, struct pcap_stat *);
	void	(*close_op)(pcap_t *);

	/*
	 * Placeholder for filter code if bpf not in kernel.
	 */
	struct bpf_program fcode;

	char errbuf[PCAP_ERRBUF_SIZE + 1];
	int dlt_count;
	int *dlt_list;

	struct pcap_pkthdr pcap_header;	/* This is needed for the pcap_next_ex() to work */

#ifdef HAVE_REMOTE
#ifndef WIN32	// Win32 already defines 'timeout'
	int timeout;				//!< timeout to be used in the pcap_open()
#endif
	/*! \brief '1' if we're the network client; needed by several functions (like pcap_setfilter() ) to know if 
		they have to use the socket or they have to open the local adapter. */
	int rmt_clientside;

	SOCKET rmt_sockctrl;		//!< socket ID of the socket used for the control connection
	SOCKET rmt_sockdata;		//!< socket ID of the socket used for the data connection
	int rmt_flags;				//!< we have to save flags, since they are passed by the pcap_open_live(), but they are used by the pcap_startcapture()
	int rmt_capstarted;			//!< 'true' if the capture is already started (needed to knoe if we have to call the pcap_startcapture()
	struct pcap_samp rmt_samp;	//!< Keeps the parameters related to the sampling process.
	char *currentfilter;		//!< Pointer to a buffer (allocated at run-time) that stores the current filter. Needed when flag PCAP_OPENFLAG_NOCAPTURE_RPCAP is turned on.
#endif /* HAVE_REMOTE */
};