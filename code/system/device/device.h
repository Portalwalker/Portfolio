#ifndef DEVICE_H
#define DEVICE_H



#include "dep/unite.h"



void devifaddrs();



#define NETLINK_ROUTE           0    /* Routing/device hook                */
#define NETLINK_UNUSED          1    /* Unused number                */
#define NETLINK_USERSOCK        2    /* Reserved for user mode socket protocols     */
#define NETLINK_FIREWALL        3    /* Unused number, formerly ip_queue        */
#define NETLINK_SOCK_DIAG       4    /* socket monitoring                */
#define NETLINK_NFLOG           5    /* netfilter/iptables ULOG */
#define NETLINK_XFRM            6    /* ipsec */
#define NETLINK_SELINUX         7    /* SELinux event notifications */
#define NETLINK_ISCSI           8    /* Open-iSCSI */
#define NETLINK_AUDIT           9    /* auditing */
#define NETLINK_FIB_LOOKUP      10
#define NETLINK_CONNECTOR       11
#define NETLINK_NETFILTER       12   /* netfilter subsystem */
#define NETLINK_IP6_FW          13
#define NETLINK_DNRTMSG         14   /* DECnet routing messages (obsolete) */
#define NETLINK_KOBJECT_UEVENT  15   /* Kernel messages to userspace */
#define NETLINK_GENERIC         16
     // NETLINK_DM              17   /* DM Events -- maybe ? ignore for now */
#define NETLINK_SCSITRANSPORT   18   /* SCSI Transports */
#define NETLINK_ECRYPTFS        19
#define NETLINK_RDMA            20
#define NETLINK_CRYPTO          21   /* Crypto layer */
#define NETLINK_SMC             22   /* SMC monitoring */
#define NETLINK_INET_DIAG       NETLINK_SOCK_DIAG


/* Flags                   */
#define NLM_F_REQUEST           0x01    /* It is request message.                       */
#define NLM_F_MULTI             0x02    /* Multipart message, terminated by NLMSG_DONE  */
#define NLM_F_ACK               0x04    /* Reply with ack, with zero or error code      */
#define NLM_F_ECHO              0x08    /* Receive resulting notifications              */
#define NLM_F_DUMP_INTR         0x10    /* Dump was inconsistent due to sequence change */
#define NLM_F_DUMP_FILTERED     0x20    /* Dump was filtered as requested               */

/* Modifiers to GET req    */
#define NLM_F_ROOT              0x100    /* specify tree root   */
#define NLM_F_MATCH             0x200    /* return all matching */
#define NLM_F_ATOMIC            0x400    /* atomic GET          */
#define NLM_F_DUMP              (NLM_F_ROOT|NLM_F_MATCH)

/* Modifiers to NEW req    */
#define NLM_F_REPLACE           0x100    /* Override existing            */
#define NLM_F_EXCL              0x200    /* Do not touch, if it exists   */
#define NLM_F_CREATE            0x400    /* Create, if it does not exist */
#define NLM_F_APPEND            0x800    /* Add to end of list           */

/* Modifiers to DELETE req */
#define NLM_F_NONREC            0x100    /* Do not delete recursively */
#define NLM_F_BULK              0x200    /* Delete multiple objects   */

/* Flags for ACK message   */
#define NLM_F_CAPPED            0x100    /* request was capped              */
#define NLM_F_ACK_TLVS          0x200    /* extended ACK TVLs were included */



#define RTM_BASE    16
#define RTM_NEWLINK 16
#define RTM_DELLINK 17
#define RTM_GETLINK 18
#define RTM_SETLINK 19

#define RTM_NEWADDR 20
#define RTM_DELADDR 21
#define RTM_GETADDR 22

#define RTM_NEWROUTE 24
#define RTM_DELROUTE 25
#define RTM_GETROUTE 26

#define RTM_NEWNEIGH 28
#define RTM_DELNEIGH 29
#define RTM_GETNEIGH 30

#define RTM_NEWRULE 32
#define RTM_DELRULE 33
#define RTM_GETRULE 34

#define RTM_NEWQDISC 36
#define RTM_DELQDISC 37
#define RTM_GETQDISC 38

#define RTM_NEWTCLASS 40
#define RTM_DELTCLASS 41
#define RTM_GETTCLASS 42

#define RTM_NEWTFILTER 44
#define RTM_DELTFILTER 45
#define RTM_GETTFILTER 46

#define RTM_NEWACTION 48
#define RTM_DELACTION 49
#define RTM_GETACTION 50

#define RTM_NEWPREFIX  52

#define RTM_NEWMULTICAST 56
#define RTM_DELMULTICAST 57
#define RTM_GETMULTICAST 58

#define RTM_NEWANYCAST 60
#define RTM_DELANYCAST 61
#define RTM_GETANYCAST 62

#define RTM_NEWNEIGHTBL 64
#define RTM_GETNEIGHTBL 66
#define RTM_SETNEIGHTBL 67

#define RTM_NEWNDUSEROPT 68

#define RTM_NEWADDRLABEL 72
#define RTM_DELADDRLABEL 73
#define RTM_GETADDRLABEL 74

#define RTM_GETDCB 78
#define RTM_SETDCB 79

#define RTM_NEWNETCONF 80
#define RTM_DELNETCONF 81
#define RTM_GETNETCONF 82

#define RTM_NEWMDB 84
#define RTM_DELMDB 85
#define RTM_GETMDB 86

#define RTM_NEWNSID 88
#define RTM_DELNSID 89
#define RTM_GETNSID 90

#define RTM_NEWSTATS 92
#define RTM_GETSTATS 94
#define RTM_SETSTATS 95

#define RTM_NEWCACHEREPORT 96

#define RTM_NEWCHAIN 100
#define RTM_DELCHAIN 101
#define RTM_GETCHAIN 102

#define RTM_NEWNEXTHOP 104
#define RTM_DELNEXTHOP 105
#define RTM_GETNEXTHOP 106

#define RTM_NEWLINKPROP 108
#define RTM_DELLINKPROP 109
#define RTM_GETLINKPROP 110

#define RTM_NEWVLAN 112
#define RTM_DELVLAN 113
#define RTM_GETVLAN 114

#define RTM_NEWNEXTHOPBUCKET 116
#define RTM_DELNEXTHOPBUCKET 117
#define RTM_GETNEXTHOPBUCKET 118

#define RTM_NEWTUNNEL 120
#define RTM_DELTUNNEL 121
#define RTM_GETTUNNEL 122



#define IF_NAMESIZE 16



#define IFA_UNSPEC           0
#define IFA_ADDRESS          1
#define IFA_LOCAL            2
#define IFA_LABEL            3
#define IFA_BROADCAST        4
#define IFA_ANYCAST          5
#define IFA_CACHEINFO        6
#define IFA_MULTICAST        7
#define IFA_FLAGS            8
#define IFA_RT_PRIORITY      9  /* u32, priority/metric for prefix route */
#define IFA_TARGET_NETNSID  10
#define IFA_PROTO           11  /* u8, address protocol */
#define IFA_MC_USERS        12  /* u32, multicast group users */



typedef struct {
    u16 nl_family;
    u16 nl_pad;
    u32 nl_pid;
    u32 nl_groups;
} nlsock;

typedef struct {
    u32 msg_len;
    u16 msg_type;
    u16 msg_flags;
    u32 msg_seq;
    u32 msg_pid;
} nlhead;

typedef struct
{
    unsigned short rta_len;
    unsigned short rta_type;
} rtattr;

typedef struct
{
     u8 ifa_family;     /* IPv4 or IPv6      */
     u8 ifa_prefixlen;  /* The prefix length */
     u8 ifa_flags;      /* Flags             */
     u8 ifa_scope;      /* Address scope     */
    u32 ifa_index;      /* Link index        */
} ifaddrmsg;

typedef struct
{
    void* iov_base;
   size_t iov_len;
} iovec;

typedef struct
{
    void* msg_name;      /* ptr to socket address structure */
     int  msg_namelen;        /* size of socket address structure */
   iovec* msg_iov;    /* scatter/gather array */
  size_t  msg_iovlen;        /* # elements in msg_iov */
    void* msg_control;    /* ancillary data */
  size_t  msg_controllen;        /* ancillary data buffer length */
     int  msg_flags;        /* flags on received message */
} msghdr;



#define NLMSG_NOOP      0x01    /* Nothing.                          */
#define NLMSG_ERROR     0x02    /* Error                             */
#define NLMSG_DONE      0x03    /* End of a dump                     */
#define NLMSG_OVERRUN   0x04    /* Data lost                         */
#define NLMSG_MIN_TYPE  0x10    /* < 0x10: reserved control messages */

#define NLMSG_ALIGNTO                  4U

#define NLMSG_ALIGN(length)            (((length)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1))

#define NLMSG_HDRLEN                   cast(NLMSG_ALIGN(sizeof(nlhead)), int)

#define NLMSG_LENGTH(length)           ((length) + NLMSG_HDRLEN)

#define NLMSG_SPACE(length)            NLMSG_ALIGN(NLMSG_LENGTH(length))

#define NLMSG_DATA(header)             voidptr(u8ptr(header) + NLMSG_HDRLEN)

#define NLMSG_NEXT(header, length)     ((length) -= NLMSG_ALIGN((header)->msg_len), cast(u8ptr(header) + \
                                                                                         NLMSG_ALIGN((header)->msg_len), nlhead*))

#define NLMSG_OK(header, length)       ((length) >= sizeof(nlhead)          && \
                                        (header)->msg_len >= sizeof(nlhead) && \
                                        (header)->msg_len <= (length))

#define NLMSG_PAYLOAD(header, length)  ((header)->msg_len - NLMSG_SPACE((length)))



#define RTA_ALIGNTO              4U

#define RTA_ALIGN(length)        ( ((length)+RTA_ALIGNTO-1) & ~(RTA_ALIGNTO-1) )

#define RTA_OK(rta, length)      ((length) >= cast(sizeof(rtattr), int) && \
                                  (rta)->rta_len >= sizeof(rtattr)      && \
                                  (rta)->rta_len <= (length))

#define RTA_NEXT(rta, attrlen)   ((attrlen) -= RTA_ALIGN((rta)->rta_len), \
                                                         cast(u8ptr(rta) + RTA_ALIGN((rta)->rta_len), rtattr*))

#define RTA_LENGTH(length)       (RTA_ALIGN(sizeof(rtattr)) + (length))

#define RTA_SPACE(length)        RTA_ALIGN(RTA_LENGTH(length))

#define RTA_DATA(rta)            voidptr(u8ptr(rta) + RTA_LENGTH(0))

#define RTA_PAYLOAD(rta)         (cast((rta)->rta_len, int) - RTA_LENGTH(0))



#define IFA_RTA(r)      cast(u8ptr(r) + NLMSG_ALIGN(sizeof(ifaddrmsg)), rtattr*)
#define IFA_PAYLOAD(n)  NLMSG_PAYLOAD((n), sizeof(ifaddrmsg))



#endif // DEVICE_H
