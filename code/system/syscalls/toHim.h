#ifndef TO_HIM
#define TO_HIM




#include "dep/types.h"
#include "dep/arch.h"
#include "dep/os.h"
#include "dep/bits.h"
#include "dep/constructions.h"
#include "dep/principles.h"
#include "dep/promise.h"
#include "dep/debug.h"

#include "dep/lseek.h"
#include "dep/mmap.h"
#include "dep/open.h"
#include "dep/time.h"




#if defined(ARCH_X8664)

   #define SYSCALL_BIND               49
   #define SYSCALL_CLOCK_GETTIME     228
   #define SYSCALL_CLOSE               3
   #define SYSCALL_EXIT               60
   #define SYSCALL_GETPID             39
   #define SYSCALL_IOCTL              16
   #define SYSCALL_LSEEK               8
   #define SYSCALL_MLOCK             149
   #define SYSCALL_MMAP                9
   #define SYSCALL_MUNMAP             11
   #define SYSCALL_OPEN                2
   #define SYSCALL_READ                0
   #define SYSCALL_RECVFROM           45
   #define SYSCALL_RECVMSG            47
   #define SYSCALL_SENDMSG            46
   #define SYSCALL_SENDTO             44
   #define SYSCALL_SOCKET             41
   #define SYSCALL_WRITE               1





#elif defined(ARCH_ARM64)

     #define SYSCALL_BIND              200
     #define SYSCALL_CLOCK_GETTIME     113
     #define SYSCALL_CLOSE              57
     #define SYSCALL_EXIT               93
     #define SYSCALL_GETPID            172
     #define SYSCALL_IOCTL              29
     #define SYSCALL_LSEEK              62
     #define SYSCALL_MLOCK             228
     #define SYSCALL_MMAP              222
     #define SYSCALL_MUNMAP            215
     #define SYSCALL_OPENAT             56
     #define SYSCALL_READ               63
     #define SYSCALL_RECVFROM          207
     #define SYSCALL_RECVMSG           212
     #define SYSCALL_SENDMSG           211
     #define SYSCALL_SENDTO            206
     #define SYSCALL_SOCKET            198
     #define SYSCALL_WRITE              64





#elif defined(ARCH_I386)

     #define SYSCALL_BIND              361
     #define SYSCALL_CLOCK_GETTIME     265
     #define SYSCALL_CLOCK_GETTIME64   403 // for time accuracy post-2038
     #define SYSCALL_CLOSE               6
     #define SYSCALL_EXIT                1
     #define SYSCALL_GETPID             20
     #define SYSCALL_IOCTL              54
     #define SYSCALL_LSEEK              19
     #define SYSCALL_MLOCK             150
     #define SYSCALL_MMAP              192 // is actually mmap2
     #define SYSCALL_MUNMAP             91
     #define SYSCALL_OPEN                5
     #define SYSCALL_READ                3
     #define SYSCALL_RECVFROM          371
     #define SYSCALL_RECVMSG           372
     #define SYSCALL_SENDMSG           370
     #define SYSCALL_SENDTO            369
     #define SYSCALL_SOCKET            359
     #define SYSCALL_SOCKETCALL        102
     #define SYSCALL_WRITE               4





#elif defined(ARCH_ARMEABI)

     #define SYSCALL_BIND              282
     #define SYSCALL_CLOCK_GETTIME     263
     #define SYSCALL_CLOCK_GETTIME64   403 // for time accuracy post-2038
     #define SYSCALL_CLOSE               6
     #define SYSCALL_EXIT                1
     #define SYSCALL_GETPID             20
     #define SYSCALL_IOCTL              54
     #define SYSCALL_LSEEK              19
     #define SYSCALL_MLOCK             150
     #define SYSCALL_MMAP              192 // this actually mmap2
     #define SYSCALL_MUNMAP             91
     #define SYSCALL_OPENAT            322
     #define SYSCALL_READ                3
     #define SYSCALL_RECVFROM          292
     #define SYSCALL_RECVMSG           297
     #define SYSCALL_SENDMSG           296
     #define SYSCALL_SENDTO            290
     #define SYSCALL_SOCKET            281
     #define SYSCALL_WRITE               4





#elif defined(ARCH_ARMOABI)

     #define SYSCALL_BIND              9437466
     #define SYSCALL_CLOCK_GETTIME     9437447
     #define SYSCALL_CLOCK_GETTIME64   9437587 // for time accuracy post-2038
     #define SYSCALL_CLOSE             9437190
     #define SYSCALL_EXIT              9437185
     #define SYSCALL_GETPID            9437204
     #define SYSCALL_IOCTL             9437238
     #define SYSCALL_LSEEK             9437203
     #define SYSCALL_MLOCK             9437334
     #define SYSCALL_MMAP              9437376 // this actually mmap2
     #define SYSCALL_MUNMAP            9437275
     #define SYSCALL_OPEN              9437189
     #define SYSCALL_OPENAT            9437506
     #define SYSCALL_READ              9437187
     #define SYSCALL_RECVFROM          9437476
     #define SYSCALL_RECVMSG           9437481
     #define SYSCALL_SENDMSG           9437480
     #define SYSCALL_SENDTO            9437474
     #define SYSCALL_SOCKET            9437465
     #define SYSCALL_SOCKETCALL        9437286
     #define SYSCALL_WRITE             9437188




#elif defined(ARCH_MIPS_N64)

     #define SYSCALL_BIND              5048
     #define SYSCALL_CLOCK_GETTIME     5222
     #define SYSCALL_CLOSE             5003
     #define SYSCALL_EXIT              5058
     #define SYSCALL_GETPID            5038
     #define SYSCALL_IOCTL             5015
     #define SYSCALL_LSEEK             5008
     #define SYSCALL_MLOCK             5146
     #define SYSCALL_MMAP              5009
     #define SYSCALL_MUNMAP            5011
     #define SYSCALL_OPEN              5002
     #define SYSCALL_OPENAT            5247
     #define SYSCALL_READ              5000
     #define SYSCALL_RECVFROM          5044
     #define SYSCALL_RECVMSG           5046
     #define SYSCALL_SENDMSG           5045
     #define SYSCALL_SENDTO            5043
     #define SYSCALL_SOCKET            5040
     #define SYSCALL_WRITE             5001






#elif defined(ARCH_MIPS_N32)

     #define SYSCALL_BIND              6048
     #define SYSCALL_CLOCK_GETTIME     6226
     #define SYSCALL_CLOCK_GETTIME64   6403 // for time accuracy post-2038
     #define SYSCALL_CLOSE             6003
     #define SYSCALL_EXIT              6058
     #define SYSCALL_GETPID            6038
     #define SYSCALL_IOCTL             6015
     #define SYSCALL_LSEEK             6008
     #define SYSCALL_MLOCK             6146
     #define SYSCALL_MMAP              6009
     #define SYSCALL_MUNMAP            6011
     #define SYSCALL_OPEN              6002
     #define SYSCALL_OPENAT            6251
     #define SYSCALL_READ              6000
     #define SYSCALL_RECVFROM          6044
     #define SYSCALL_RECVMSG           6046
     #define SYSCALL_SENDMSG           6045
     #define SYSCALL_SENDTO            6043
     #define SYSCALL_SOCKET            6040
     #define SYSCALL_WRITE             6001





#elif defined(ARCH_MIPS_O32)

     #define SYSCALL_BIND              4169
     #define SYSCALL_CLOCK_GETTIME     4263
     #define SYSCALL_CLOCK_GETTIME64   4403 // for time accuracy post-2038
     #define SYSCALL_CLOSE             4006
     #define SYSCALL_EXIT              4001
     #define SYSCALL_GETPID            4020
     #define SYSCALL_IOCTL             4054
     #define SYSCALL_LSEEK             4019
     #define SYSCALL_MLOCK             4154
     #define SYSCALL_MMAP              4090
     #define SYSCALL_MMAP              4210 // this actually mmap2
     #define SYSCALL_MUNMAP            4091
     #define SYSCALL_OPEN              4005
     #define SYSCALL_OPENAT            4288
     #define SYSCALL_READ              4003
     #define SYSCALL_RECVFROM          4176
     #define SYSCALL_RECVMSG           4177
     #define SYSCALL_SENDMSG           4179
     #define SYSCALL_SENDTO            4180
     #define SYSCALL_SOCKET            4183
     #define SYSCALL_SOCKETCALL        4102
     #define SYSCALL_WRITE             4004

#else

    #error "Unsupported architecture"

#endif // ARCHITECTURE CHECKS





// ABSTRACTIONS

                        #define MAX_SYSCALL_ARGS (6+1) // for alignment
long system_call(  long,  long[ MAX_SYSCALL_ARGS ]  );

#define system_call_abstraction(need, ...) system_call((need), ( long [MAX_SYSCALL_ARGS]){ __VA_ARGS__ })





/* **************** *
 * REGULAR SYSCALLS *
 * **************** */

#define      close(descriptor)                   system_call_abstraction(SYSCALL_CLOSE,          (long)(descriptor))
#define       exit(code)                         system_call_abstraction(SYSCALL_EXIT,           (long)(code))
#define      lseek(descriptor, offset, whence)   system_call_abstraction(SYSCALL_LSEEK,          (long)(descriptor),  (long)(offset),  (long)(whence))
#define      mlock(address, amount)              system_call_abstraction(SYSCALL_MLOCK,          (long)(address),     (long)(amount))
#define     munmap(address, amount)              system_call_abstraction(SYSCALL_MUNMAP,         (long)(address),     (long)(amount))
#define       read(descriptor, buffer, amount)   system_call_abstraction(SYSCALL_READ,           (long)(descriptor),  (long)(buffer),  (long)(amount))
#define socketcall(call, args)                   system_call_abstraction(SYSCALL_SOCKETCALL,     (long)(call),        (long)(args))
#define     socket(domain, level, protocol)      system_call_abstraction(SYSCALL_SOCKET,         (long)(domain),      (long)(level),   (long)(protocol))
#define      write(descriptor, buffer, amount)   system_call_abstraction(SYSCALL_WRITE,          (long)(descriptor),  (long)(buffer),  (long)(amount))



/* ******************************* *
 * #else == MOST USUAL CONVENTION  * <--- me likey
 *   #if == LESS USUAL CONVENTION  * <--- me no likey
 * ******************************* */



/* ********** *
 * MMAP/MMAP2 *
 * ********** */

#if defined(ARCH_I386)    || \
    defined(ARCH_ARMEABI) || \
    defined(ARCH_ARMOABI) || \
    defined(ARCH_MIPS_O32)

    // MMAP2
    #define  mmap(amount)  system_call_abstraction(SYSCALL_MMAP,  (long)(MMAP_ADDR), (long)(amount),  (long)(MMAP_PROT),  (long)(MMAP_FLAGS),  (long)(MMAP_DESCRIPTOR),  (long)(MMAP_OFFSET >> MMAP2_PAGE_SHIFT_FOR_32BIT_ARCH))

#else

    // MMAP
    #define  mmap(amount)  system_call_abstraction(SYSCALL_MMAP,  (long)(MMAP_ADDR), (long)(amount),  (long)(MMAP_PROT),  (long)(MMAP_FLAGS),  (long)(MMAP_DESCRIPTOR),  (long)(MMAP_OFFSET))

#endif



/* ***************** *
 * CREAT/OPEN/OPENAT *
 * ***************** */

#if defined(ARCH_ARM64)

    #define  create(path)  system_call_abstraction(SYSCALL_OPENAT,  (long)(OPENAT_ARM_AT_FDCWD),  (long)(path),  (long)(CREATE_FILE_FLAGS),  (long)(CREATE_MODE))
    #define    open(path)  system_call_abstraction(SYSCALL_OPENAT,  (long)(OPENAT_ARM_AT_FDCWD),  (long)(path),  (long)(OPEN_FILE_FLAGS),    (long)(OPEN_MODE))

#else

    #define  create(path)  system_call_abstraction(SYSCALL_OPEN,                                  (long)(path),  (long)(CREATE_FILE_FLAGS),  (long)(CREATE_MODE))
    #define    open(path)  system_call_abstraction(SYSCALL_OPEN,                                  (long)(path),  (long)(OPEN_FILE_FLAGS),    (long)(OPEN_MODE))

#endif



/* ****************** *
 * LEGACY SOCKETCALLS *
 * ****************** */

#if defined(ARCH_I386) || \
    defined(ARCH_ARMOABI)

    #define socket_call_abstraction(need, ...) system_call(SYSCALL_SOCKETCALL, (need), long [MAX_SYSCALL_ARGS]{ __VA_ARGS__ })

    #define SOCKETCALL_SOCKET         1
    #define SOCKETCALL_BIND           2
    #define SOCKETCALL_CONNECT        3
    #define SOCKETCALL_LISTEN         4
    #define SOCKETCALL_ACCEPT         5
    #define SOCKETCALL_GETSOCKNAME    6
    #define SOCKETCALL_GETPEERNAME    7
    #define SOCKETCALL_SOCKETPAIR     8
    #define SOCKETCALL_SEND           9
    #define SOCKETCALL_RECV          10
    #define SOCKETCALL_SENDTO        11
    #define SOCKETCALL_RECVFROM      12
    #define SOCKETCALL_SHUTDOWN      13
    #define SOCKETCALL_SETSOCKOPT    14
    #define SOCKETCALL_GETSOCKOPT    15
    #define SOCKETCALL_SENDMSG       16
    #define SOCKETCALL_RECVMSG       17

    #define      bind(descriptor, sock, socklen)                         socket_call_abstraction(SOCKETCALL_BIND,      (long)(descriptor),  (long)(sock),     (long)(socklen))
    #define  recvfrom(descriptor, pkt, pktlen, flags, sock, socklenptr)  socket_call_abstraction(SOCKETCALL_RECVFROM,  (long)(descriptor),  (long)(pkt),      (long)(pktlen),  (long)(flags), (long)(sock), (long)(socklenptr))
    #define   recvmsg(descriptor, message, flags)                        socket_call_abstraction(SOCKETCALL_RECVMSG,   (long)(descriptor),  (long)(message),  (long)(flags))
    #define   sendmsg(descriptor, message, flags)                        socket_call_abstraction(SOCKETCALL_SENDMSG,   (long)(descriptor),  (long)(message),  (long)(flags))
    #define    sendto(descriptor, pkt, pktlen, flags, sock, socklen)     socket_call_abstraction(SOCKETCALL_SENDTO,    (long)(descriptor),  (long)(pkt),      (long)(pktlen),  (long)(flags), (long)(sock), (long)(socklen))

#else

    #define      bind(descriptor, sock, socklen)                         system_call_abstraction(SYSCALL_BIND,         (long)(descriptor),  (long)(sock),     (long)(socklen))
    #define  recvfrom(descriptor, pkt, pktlen, flags, sock, socklenptr)  system_call_abstraction(SYSCALL_RECVFROM,     (long)(descriptor),  (long)(pkt),      (long)(pktlen),  (long)(flags), (long)(sock), (long)(socklenptr))
    #define   recvmsg(descriptor, message, flags)                        system_call_abstraction(SYSCALL_RECVMSG,      (long)(descriptor),  (long)(message),  (long)(flags))
    #define   sendmsg(descriptor, message, flags)                        system_call_abstraction(SYSCALL_SENDMSG,      (long)(descriptor),  (long)(message),  (long)(flags))
    #define    sendto(descriptor, pkt, pktlen, flags, sock, socklen)     system_call_abstraction(SYSCALL_SENDTO,       (long)(descriptor),  (long)(pkt),      (long)(pktlen),  (long)(flags), (long)(sock), (long)(socklen))

#endif



/* *************************** *
 * CLOCK_GETTIME   (STANDARD)  *
 * CLOCK_GETTIME64 (POST-2038) *
 * *************************** */

#if defined(ARCH_I386)     || \
    defined(ARCH_ARMEABI)  || \
    defined(ARCH_ARMOABI)  || \
    defined(ARCH_MIPS_N32) || \
    defined(ARCH_MIPS_O32)

    #define  time32(clocktype, timestruct)  system_call_abstraction(SYSCALL_CLOCK_GETTIME,    (long)(clocktype),  (long)(timestruct))
    #define  time64(clocktype, timestruct)  system_call_abstraction(SYSCALL_CLOCK_GETTIME64,  (long)(clocktype),  (long)(timestruct))

#else

    #define  time32(clocktype, timestruct)  system_call_abstraction(SYSCALL_CLOCK_GETTIME,    (long)(clocktype),  (long)(timestruct))
    #define  time64(clocktype, timestruct)  system_call_abstraction(SYSCALL_CLOCK_GETTIME,    (long)(clocktype),  (long)(timestruct))

#endif

#endif // TO_HIM
