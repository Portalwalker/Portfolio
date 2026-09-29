
#include "dep/device.h"
#include "dep/trix.h"
#include "dep/arena.h"
#include "dep/sock.h"

u64 get_ip(u64 fd, nlsock* sa, u32 domain)
{
    u64 BUFLEN = PAGES(1);
    u8 buf[BUFLEN];

    memset(buf, 0, BUFLEN);

    // assemble the message according to the netlink protocol
    nlhead* header;
    header = (nlhead*)buf;
    header->msg_len = NLMSG_LENGTH(sizeof(ifaddrmsg));
    header->msg_type = RTM_GETADDR;
    header->msg_flags = NLM_F_REQUEST | NLM_F_ROOT;

    ifaddrmsg* ifa = cast(NLMSG_DATA(header), ifaddrmsg*);
    ifa->ifa_family = domain; // we only get ipv4 address here

    // prepare struct msghdr for sending.
    iovec iov =  { header, header->msg_len };
    msghdr msg = { sa, sizeof(*sa), &iov, 1, nullptr, 0, 0 };

    // send netlink message to kernel.
    u32 r = sendmsg(fd, &msg, 0);
    return (r < 0) ? ERROR : 0;
}

u32 get_msg(u64 fd, nlsock *sa, u8* buf, u64 len)
{
    iovec iov;
    msghdr msg;
    iov.iov_base = buf;
    iov.iov_len = len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = sa;
    msg.msg_namelen = sizeof(*sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    return recvmsg(fd, &msg, 0);
}

u32 parse_ifa_msg(ifaddrmsg* ifa, void* buf, u64 len)
{
    u8 ifname[IF_NAMESIZE];
    //printf("==================================\n");
    //printf("family:\t\tIPv%d\n", (ifa->ifa_family == AF_INET) ? 4 : 6);
    //printf("dev:\t\t%s\n", if_indextoname(ifa->ifa_index, ifname));
    //printf("prefix length:\t%d\n", ifa->ifa_prefixlen);
    //printf("\n");

    rtattr* rta = nullptr;
    u32 fa = ifa->ifa_family;
    for (rta = (rtattr*)buf; RTA_OK(rta, len); rta = RTA_NEXT(rta, len))
    {
        if (rta->rta_type == IFA_ADDRESS) {
            //printf("if address:\t%s\n", ntop(fa, RTA_DATA(rta)));
        }

        if (rta->rta_type == IFA_LOCAL) {
            //printf("local address:\t%s\n", ntop(fa, RTA_DATA(rta)));
        }

        if (rta->rta_type == IFA_BROADCAST) {
            //printf("broadcast:\t%s\n", ntop(fa, RTA_DATA(rta)));
        }
    }

    return 0;
}

u64 parse_nl_msg(void* buf, u64 len)
{
    nlhead* header = nullptr;

    for (header = (nlhead*)buf; NLMSG_OK(header, len) && header->msg_type != NLMSG_DONE; header = NLMSG_NEXT(header, len))
    {
        if (header->msg_type == NLMSG_ERROR)
        {
            return ERROR;
        }

        if (header->msg_type == RTM_NEWADDR)
        {
            ifaddrmsg *ifa = cast(NLMSG_DATA(header), ifaddrmsg*);
            parse_ifa_msg(ifa, IFA_RTA(ifa), IFA_PAYLOAD(header));
            continue;
        }
    }

    return header->msg_type;
}

/*
void devifaddrs()
{
    if (startgrid() == EVIL) { exit(1); }

    u64 fd = 0;
    u64 len = 0;
    u64 BUFLEN = PAGES(1);
    u64 nl_msg_type = 0;

    // First of all, we need to create a socket with the AF_NETLINK domain
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    nlsock sa;
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;

    len = get_ip(fd, &sa, AF_INET); // To get ipv6, use AF_INET6 instead

    u64  a1 = arena_map(BUFLEN);
    u8* buf = arena_start(a1);

    do
    {
        len = get_msg(fd, &sa, buf, arena_width(a1));

        nl_msg_type = parse_nl_msg(buf, len);
    } while (nl_msg_type != NLMSG_DONE && nl_msg_type != NLMSG_ERROR);

    exit(endgrid() == EVIL);
} */
