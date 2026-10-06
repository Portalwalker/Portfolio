#ifndef PRINCIPLES_H
#define PRINCIPLES_H

#include "dep/types.h"

#define TRUE  1
#define FALSE 0

#define GOOD  TRUE
#define EVIL  FALSE

#define SUCCESS GOOD
#define FAILURE EVIL

#define ERROR u64c(-1)

#define NULL_DESCRIPTOR ERROR
#define STDIN  0
#define STDOUT 1
#define STDERR 2

#define STACK_ALIGNMENT 16

#define KILOBYTE 1024
#define MEGABYTE u64c(KILOBYTE * 1024)
#define GIGABYTE u64c(MEGABYTE * 1024)

#define PAGESIZE       4096
#define PAGES(num)     (PAGESIZE * (num))
#define AMT2PAGES(amt) u64c( ((amt) / PAGESIZE) + (((amt) % PAGESIZE) != 0) )

#define SECTORSIZE      512
#define AMT2SECTOR(amt) u64c( ((amt) / SECTORSIZE) + (((amt) % SECTORSIZE) != 0) )
#define SECTORS(num)    u64c(SECTORSIZE * (num))

#endif // PRINCIPLES_H
