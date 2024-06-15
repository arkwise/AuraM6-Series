////////////////////////////////////////////////////////////////////////////////
//
//	Ping
//	A simple ping program to demonstrate TCP support
//
//	Copyright (c) 2011 Chase Finn & Finn Technologies
//
////////////////////////////////////////////////////////////////////////////////
#include <conio.h>
#include <tcp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wtypes.h>
#include <netinet/ip_icmp.h>

#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "tbox.h"
#include "iodlg.h"
#include "toolbar.h"

char AppName[]		= "Ping";
l_uid	nUID = "app:O3pad";
l_uid NeededLibs[] = { "menu","button","iodlg","window","textbox","toolbar","" };


#define  MSG_FILEOPEN	0xF0020002
#define  MSG_FILESAVE	0xF0020003
#define  MSG_FILESAVEAS	0xF0020004

PButton b = 0;
PTextbox tb = 0;
PTextbox tbip = 0;
PWindow w = 0;
PToolbar bar = 0;
PFileTypes Filter = 0;
l_text OpenedFile = 0;
l_text textboxip = 0;
PLabel lab;
PLabel lba;

int fin_delay = -1;
int is_fin = 0;
int datalen = -1;
int tell_all = 0;



  /* Host name lookup...
   *
   *   Resolve host name using domain resolver or whatever, and copy 
   *   canonical host name into canonp[canonl].  But if
   *   argument is dotted-decimal string (first char is digit), just
   *   convert it without lookup (we don't want to fail just because
   *   the INADDR database is not complete).  
   *
   */
u_long resolve_name( namep, canonp, canonl )
 char *namep, canonp[] ;
 int  canonl ;
    {
    struct hostent *hp ;
    u_long inetaddr ;
    int n ;
    
    if (isdigit(*namep)) {
            /* Assume dotted-decimal */
        inetaddr = (u_long) inet_addr(namep) ;
        *canonp = '\0' ;   /* No canonical name */
        return(inetaddr) ;
      }
    else  {
        if (NULL == (hp =  gethostbyname( namep )))
            return(0) ;
        n = ((n = strlen(hp->h_name)) >= canonl) ?  canonl-1 : n ;
        bcopy(hp->h_name, canonp, n) ;
        hp->h_name[n] = '\0' ;
         
        return( *( u_long *) hp->h_addr) ;
    }
}


/*
 *          M A I N
 */
main_ping_sys(argc, argv)
int argc;
char *argv[];
{
    register int i, ch ;
    u_long options;
    char   tod_buf[26] ; 

    while ((ch = getopt(argc, argv, "xd:t:b:n:e:uf:r:h:?")) != EOF)
      switch (ch)
      {
        case 'x':
            options |= SO_DEBUG;
            tell_all = 1 ;
            break ;
            
        case 'd':
            datalen = atoi(optarg);
            if( datalen > MAXPACKET)  {
                printf("ping:  packet size too big\n");
                exit(1);
            }
            totallen = datalen + ICMP_MINLEN;
            break ;
            
        case 't':
            interval = atoi(optarg) ;
            break ;
            
        case 'b':
            burst_size = atoi(optarg) ;
            break ;
            
        case 'n':
            send_limit = atoi(optarg) ;
            break ;

        case 'e':
            echo_host = 1 ;
            if (0 == strcmp(2+*argv, "raw")) RAW_echo = 1 ;
            break ;

        case 'u':
        case 'f':
        case 'r':
        case 'h':
             Get_Parm (ch,optarg);
             break;

        case '?':
        default :
             printf ("Usage: %s %s", argv[0], usage);
             exit (0);
      }

    argc -= optind;
    argv += optind;

    if (argc <= 0) {
        printf("Missing Target Host\n") ;
        exit(1) ;
    }

    if (burst_size > MAXBURST) {
        printf("Burst size max is %d\n", MAXBURST) ;
        return(1);
    }        
    if (datalen < sizeof(struct timeval)) {
        printf("NOTE:  Data length reset to min = 8 bytes\n") ;
        datalen = sizeof(struct timeval) ;
    }

    Initialize(); /* Perform stat-specific initialization */

        /*
         *  Convert host name/number to IP address
         */
    target_host =  resolve_name( *argv, canhost, 64);
    if (target_host == 0)  {
        printf("ping: unknown host %s\n", *argv);
        return;
    }
    strcpy(hostaddr, inet_ntoa(target_host));
    target_sin.sin_addr.s_addr = target_host;
    target_sin.sin_family = AF_INET;
    
    gettimeofday(&startod, &tz) ;
    strcpy(tod_buf, ctime(&startod.tv_sec) ) ;

    printf("---PING Host= [%s]: %s echo %d data bytes\n %.24s %s\n", 
         hostaddr, (RAW_echo)? "Raw": "ICMP", datalen,
         tod_buf, canhost);

    do_ping() ;

} /* main() */

/*
 *      T E R M I N A T E
 *
 *  User has entered ^C.  Set termination flag to inhibit further
 *  pings, and delay to await stragglers.
 *
 */
void terminate()
 {
    int delay  ;
    struct timezone tz ;
    struct itimerval itim, otim ; /* time interval structures */
    extern int tmax;  /* maximum RTT */
    
    gettimeofday(&SIGINTime, &tz) ;   /* time now... */
    
    /* Wait twice the max delay, or 20 seconds, whichever is
     * less.
     */
    delay = (2*tmax + 500)/1000 ;
    itim.it_value.tv_sec =  (delay > 20) ?  20 : (delay < 2) ? 1 : delay ;
    itim.it_value.tv_usec =  0 ;
    itim.it_interval.tv_sec = itim.it_interval.tv_usec = 0 ;
                                           /* then disable*/
    setitimer(ITIMER_REAL, &itim, &otim) ; /* Start interval timer */
    is_fin = 1 ;  /* Enter finish-up mode. */
    
    printf("... Delay %d seconds for stragglers...\n", itim.it_value.tv_sec) ;
    fin_delay = itim.it_value.tv_sec * 1000 ;
    fflush(stdout) ;
    
        /* Another control C and we will quit anyway... */
    signal( SIGINT, (void(*)(int))finish );
    
}


do_ping()
{
    char packet_buff[MAXPACKET];
    int sinlen = sizeof(struct sockaddr_in);
    int i;
    u_char *datap;
    struct sockaddr_in from; 
    struct itimerval itim, otim ; /* time interval structures */

        /*
         *  Use process ID as ICMP identifier
         */
    ident = getpid() & 0xFFFF;

        /* 
         * Generate data in send packet buffer 
         */
    datap = &outpack[ICMP_MINLEN+sizeof(struct timeval)];
    for( i= sizeof( struct timeval); i<datalen; i++)    /* skip time */
        *datap++ = i;

        /* 
         * Set up interval timer structure
         */
    itim.it_interval.tv_sec = itim.it_value.tv_sec = interval/1000 ;
    itim.it_interval.tv_usec = itim.it_value.tv_usec = 
                                              (interval%1000)*1000 ;

    if ((raw_sock = socket(AF_INET, SOCK_RAW,
                                     (RAW_echo)? 255 : IPPROTO_ICMP)) < 0)
    {
        perror("ping: socket");
        exit(1) ;
    }

        /*
         *  Read clock for start time and start ...
         */
    gettimeofday(&startod, &tz) ;
    sendtod = startod ;    
    signal( SIGALRM, catcher );
    signal( SIGINT, terminate );
    setitimer(ITIMER_REAL, &itim, &otim) ; /* Start interval timer */

    for (;;) {
            /* 
             * MAIN LOOP ... waiting to receive packets
             */
        int fromlen = sizeof(from);
        int cc;

         cc = recvfrom(raw_sock, packet_buff, sizeof(packet_buff), 0, 
                       (struct sockaddr*)&from, &fromlen);
        if (cc < 0) {
            if( errno == EINTR )
                continue;
            perror("ping: recvfrom");
            continue;
        }
        else
            in_packet(packet_buff, cc, &from);
    }

} /* do_ping() */


/*
 *              C A T C H E R
 * 
 *     Catch interval timer expiration.
 *
 * This routine causes another ping request (or burst) to be transmitted,
 * and schedules another SIGALRM .  However, if we are in finish-up mode,
 * just call finish() to print statistics.
 * 
 */
void catcher()
{
    int i ;
    
    if (is_fin) {
        if (!finish() )   {
                /* It is possible (and in fact very likely, if interval is
                 * down near the minimum timer granularity, 20 ms) that the
                 * timer will go off and a SIGALRM will be queued while we 
                 * are processing SIGINT.  There is
                 * no way (?) to kill the SIGALRM that immediately follows
                 * exit from SIGINT, even though the SIGINT processing
                 * did a new setitimer() call.  Therefore, finish() checks
                 * for a long-enough delay, and if not returns immediately
                 * the value 0.  In that case, we activate the timer alarm
                 * again and wait again for the real timeout.
                 */
            signal( SIGALRM, catcher );
        }
    }   
    else {
        signal( SIGALRM, catcher );
        for (i=0; i< burst_size;i++) {
            if (send_limit-- > 0) {
                send_ping(raw_sock);
            }
            else {
                /* Reached limit on number to send. Pretend a ^C came */
                terminate() ;
                break ;
            }
        }
    }
} /* catcher() */

/*
 *          I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *   (based upon a version written by Mike Muuss, BRL)
 */
u_short
in_cksum(addr, len)
u_short *addr;
int len;
    {
    register int nleft = len;
    register u_short *w = addr;
    register int sum = 0;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while( nleft > 1 )  {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if( nleft == 1 )
        sum += *(u_char *)w;

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);     /* add hi 16 to low 16 */
        
    return ( (~sum) & 0xFFFF);
}

/*
 *          P I N G E R
 * 
 * Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 *
 */

send_ping(s)
    int s;
    {
    register struct icmp *icp  ;
    int i, cc;
    struct timeval *tp = (struct timeval *) &outpack[ICMP_MINLEN];

    icp = (struct icmp *) outpack ;
    icp->icmp_type = (echo_host)? ICMP_ECHOREPLY : ICMP_ECHO;
    icp->icmp_code = 0;
    icp->icmp_cksum = 0;
    icp->icmp_seq = htons(ntransmitted++);
    icp->icmp_id = htons(ident);        /* ID */

    cc = datalen + ICMP_MINLEN ;        /* total ICMP packet length */
    gettimeofday( &sendtod, &tz );
    bcopy( &sendtod, tp, sizeof(struct timeval)) ;

    /* Compute ICMP checksum here */
//  icp->icmp_cksum = htons(in_cksum( icp, cc ));
    icp->icmp_cksum = in_cksum( icp, cc );

    /* cc = sendto(s, msg, len, flags, to, tolen) */
    i = sendto( s, outpack, cc, 0, 
               (struct sockaddr*)&target_sin, sizeof(struct sockaddr_in) );

    if( i < 0 || i != cc )  {
        if( i<0 )  perror("sendto");
        printf("ping: wrote %d chars, ret=%d\n",
             cc, i );
        fflush(stdout);
    }

    Sent_ping();

} /* send_ping() */

/*
 *          P R _ T Y P E
 *
 * Convert an ICMP "type" field to a printable string.
 */
char *
pr_type( t )
register int t;
{
    static char *ttab[] = {
        "Echo Reply",
        "ICMP 1",
        "ICMP 2",
        "Dest Unreachable",
        "Source Quence",
        "Redirect",
        "ICMP 6",
        "ICMP 7",
        "Echo",
        "ICMP 9",
        "ICMP 10",
        "Time Exceeded",
        "Parameter Problem",
        "Timestamp",
        "Timestamp Reply",
        "Info Request",
        "Info Reply"
    };

    if( t < 0 || t > 16 )
        return("OUT-OF-RANGE");

    return(ttab[t]);
}

/*
 *          I N _ P A C K E T
 *
 * Process incoming ICMP packet...
 *
 */
in_packet( buf, cc, from )
    char *buf ;                  /* Pointer to packet in buffer */
    int cc;                     /*  Length of packet in buffer */
    struct sockaddr_in *from;   /* Who sent it */
    {
    register struct icmp *icp;  /* Ptr to ICMP header */
    struct timeval *tp = (struct timeval *) &outpack[ICMP_MINLEN];
    int rtt, seqno;
    struct timeval tv;
    extern send_reply();

#ifndef SUNOS4
#ifdef sun
    if  (!RAW_echo)
        icp = (struct icmp *) buf ; 
    else
#endif
#endif
        {   /* Point past IP header to ICMP header */
        int hlen ;
        struct ip *ip = (struct ip *) buf;
        
        hlen = ip->ip_hl << 2;
        cc -= hlen ;
        icp = (struct icmp *) (buf + hlen );
    }
    
    if( icp->icmp_type != ICMP_ECHOREPLY )  {
        
        if ( icp->icmp_type == ICMP_REDIRECT )  {
            printf("!Redirect to gateway: %s\n", 
                     inet_ntoa(icp->icmp_hun.ih_gwaddr )) ;
              /* should check that Redirect was for our packet */
            return ;
        }
            /* Unknown ICMP type.  Dump packet if -x was set. */
        if (tell_all) {
            printf("%d bytes from %s: ", cc, 
                                     inet_ntoa( from->sin_addr.s_addr) );
            printf("icmp type=%d (%s) code=%d\n",
                icp->icmp_type, pr_type(icp->icmp_type), icp->icmp_code );
            hex_dump(icp, (cc < 48)? cc : 48) ;
        }
    }
        
    if( ntohs(icp->icmp_id)!= ident )
        return;         /* 'Twas not our ECHO */
    
    gettimeofday( &tv, &tz );

    tp = (struct timeval *) &icp->icmp_dun ;  /* pt to xmit-time in packet*/
    tvsub( &tv, tp );          /* RTT for this packet */
    rtt = tv.tv_sec*1000000 + tv.tv_usec;
    seqno = ntohs(icp->icmp_seq);

    Do_ping_reply(rtt, seqno);
} /* in_packet() */


int finish()
{
    int actual_delay ;
    struct timezone tz ;
    struct timeval fintime;
    
    signal( SIGINT, SIG_DFL );  /* Restore default action */
    
        /* Determine the actual delay (to let stragglers show up), and if
         *   it is too short (due to spurious clock interrupt -- see the
         *   comment in catcher()), just return to await the real timeout.
         */
    gettimeofday(&fintime, &tz) ;
    tvsub(&fintime, &SIGINTime) ;
    actual_delay = fintime.tv_sec*1000 + fintime.tv_usec/1000  ;

       /* If this was spurious timeout, return 0 for retry. */
    if (actual_delay < fin_delay - 20)
        return(0) ;
    
    Final_summary();

    fflush(stdout);
    exit(0);
}
            
hex_dump(p, len)
register char *p ;
register int len ;
    {
    register int i = 0, j ;
    u_long  temp ;
    
    while (len > 0) {
        printf("x%2.2x: ", i );
        for (j = 0; j<4; j++)  {
            if (len > 4) {
#ifdef vax
                temp = * (u_long *) p ;
                printf(" x%8.8x", ntohl( temp )) ;
#else
                printf(" x%8.8x", * (long *) p) ;
#endif
                p += sizeof(long) ;
                i += sizeof(long) ;
                len -= sizeof(long) ;
            }
            else {
                printf(" x%*.*x",  2*len, 2*len, * (long *) p) ;
                len = 0 ;
                break ;
            }
        }
        printf("\n") ;
    }
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
				CloseApp(&Me);
				return true;
			}
			break;

			case MSG_FILESAVEAS:
			{
				l_text textboxip;


				if ( OpenedFile )
				{
					l_text txt = TextDup(tb->Text);

					if ( SaveData2(OpenedFile, txt, TYPE_TEXT ) )
					{
						WindowSetCaption(w,"Type Writer");
					}

					free(txt);
				}
				return true;
			}
			break;

			case MSG_FILEOPEN:
			{
				l_text tmp = OpenedFile;

				OpenedFile = IOBox("Open text file (UTF-8)", IOBOX_OPEN, NULL, Filter, true);

				if ( OpenedFile )
				{
					l_text txt = LoadData2(OpenedFile,TYPE_TEXT);

					if ( txt )
					{
						TextBoxSetTextEx(tb,txt);
						free(txt);
						WindowSetCaption(w,"Type Writer");
					}

					free(tmp);
				}
				return true;
			}
			break;

			case MSG_FILESAVE:


			case WM_ABOUT:
			{
				MessageBox(&Me, "About Ping", "Ping\nA Networking Demo\n\nCopyright (c) 2011 Chase Finn & Finn Technologies. All rights reserved.", MBB_OK);
				return true;
			}
			break;
		}
	}

	return false;
}



l_int Main ( int argc, l_text *argv )
{

	TRect r;


	RectAssign(&r,0, 0, 540, 380);
	w = CreateWindow(&Me, r, "Ping", WF_NORMAL|WF_CENTERED|WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	Filter = 	NewFileTypes("Text files","txt",
						NewFileTypes("All files",NULL,
						NULL ));



	//RectAssign(&r,0,0,540,20);
	//PMenuView o = NewMenuView(&Me,r,Menu,MenuViewStyleHorizontal,0);
	//InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r, 5, 0, 535, 10);
	lab = CreateLabel(&Me,r,"Enter the IP address to ping below:");
	InsertWidget(WIDGET(w), WIDGET(lab));

	RectAssign(&r,10,20,490,40);
	tbip = CreateTextbox(&Me, r, TBF_EDITABLE);
	InsertWidget(WIDGET(w), WIDGET(tbip));

	RectAssign(&r, 495, 20, 530, 40);
	b = CreateButton(&Me,r,"Ping It",WM_ABOUT);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 5, 55, 535, 65);
	lba = CreateLabel(&Me,r,"The results of the ping below:");
	InsertWidget(WIDGET(w), WIDGET(lba));

	RectAssign(&r,10,75,530,370);
	tb = CreateTextbox(&Me, r, TBF_MULTILINE);
	WIDGET(tb)->Flags |= WF_AUTORESIZE|WF_MAGNETRIGHT|WF_MAGNETBOTTOM;
	InsertWidget(WIDGET(w), WIDGET(tb));


	WidgetDrawAll(WIDGET(w));
	return true;
}

void Close (void)
{
	if ( OpenedFile ) free(OpenedFile);

	WidgetDispose(WIDGET(w));
	FreeFileTypes(Filter);
}
