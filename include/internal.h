#ifndef _INTERNAL_H_INCLUDED_
#define _INTERNAL_H_INCLUDED_

#define COMBUF_TEXT_SIZE 65536

// Be carefull using this common buffer.
// Please finish your work on it before use an other function using it.
// Funtions using common buffer text :
//   FilePrintf (vfile.c)
//   TextArgs (text.c)

extern char CommonBufferText[COMBUF_TEXT_SIZE];

extern l_ulong KernelMode;

#define KERNEL_NORMAL 	0
#define KERNEL_SAFE   	1
#define KERNEL_RESETUP	2
#define KERNEL_TEXT		3



#endif /* _INTERNAL_H_INCLUDED_ */
