
/** @defgroup platform Platforms compatibilty
	Functions to keep platforms compatibility
	@{
*/

/**	@file
	Platform Header and Inline functions (provided into kernel)
*/

#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#ifdef WIN32
	#define _OZONE__WIN32_
	
	#ifndef _INLINE
	#define _INLINE       __inline
	#endif
	
#else 
	#ifdef DJGPP
		#define _OZONE__DJGPP_
	#else
		#define _OZONE__LINUX_
	#endif
	#define _INLINE       extern __inline__
#endif

#ifdef _OZONE__LINUX_

	#include <string.h>

#endif

#ifdef _OZONE__WIN32_

	#include <stdlib.h>
	#include <memory.h>
	#include <string.h>
	
	#define __asm__ _asm
	#define __volatile__ volatile
#endif
	
#ifdef _OZONE__DJGPP_
	
	#define LWP_THREAD

#endif


/**
	Incicates the current platform. Can be usefull for some system specific methods.
	@see PlatformValues
*/

extern long Platform;

/**
@defgroup PlatformValues Platform possible values
@{
*/
#define PL_DOS 0 ///< Microsoft Dos compatible operating system (DJGPP)
#define PL_WIN 1 ///< Microsoft Windows 32 bits operating system (MSVC)
#define PL_LNX 2 ///< Linux compatible operating system (GCC)
/**
@}
*/

#ifndef _OZONE__WIN32_

/*
@defgroup FarMemoryAccess
	Following functions allow you to write to far memory on all platforms
	linux and windows provides a direct acces, but on dos you need to use 
	some asm stuff wich not works on windows and linux so we use inline functions 
	that detect platform and adapt the method depending of current system.
	
	MSVC is not supported as it do not compile DynLD files, so default stuff
	is enough.
@{
*/

/**
	Set selector for __far* functions
	Similar to _farsetsel on djgpp
*/
_INLINE void __farsetsel(unsigned short selector) {
	
	if ( !Platform ) { 
		__asm__ __volatile__ ("movw %w0,%%fs"
		  :
		  : "rm" (selector));
	}
		
}


/**
	Set an unsigned char at the specified adress
	Works in far memory
	Similar to (*((unsigned char*)(addr)) = (val)); on windows and linux
	Similar to _farnspokeb on djgpp
	@see __farsetsel
*/
_INLINE void __farnspokeb(unsigned long addr, unsigned char val) {
	
	if ( !Platform ) {
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movb %b0,(%k1)"
		  :
		  : "qi" (val), "r" (addr));
	} else
		(*((unsigned char  *)(addr)) = (val));
}

/**
	Set an unsigned short at the specified adress
	Works in far memory
	Similar to (*((unsigned short  *)(addr)) = (val)); on windows and linux
	Similar to _farnspokew on djgpp
	@see __farsetsel
*/
_INLINE void __farnspokew(unsigned long addr, unsigned short val) {
	
	if ( !Platform ) {
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movw %w0,(%k1)"
		  :
		  : "ri" (val), "r" (addr));
	} else
		(*((unsigned short *)(addr)) = (val));
}

/**
	Set an unsigned long at the specified adress
	Works in far memory
	Similar to (*((unsigned long  *)(addr)) = (val)); on windows and linux
	Similar to _farnspokel on djgpp
	@see __farsetsel
*/
_INLINE void __farnspokel(unsigned long addr, unsigned long val) {
	
	if ( !Platform ) {
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movl %k0,(%k1)"
		  :
		  : "ri" (val), "r" (addr));
	} else
		(*((unsigned long  *)(addr)) = (val));
}

/**
	Read an unsigned char at the specified adress
	Works in far memory
	Similar to (*((unsigned char *)(addr))) on windows and linux
	Similar to _farnspeekb on djgpp
	@see __farsetsel
*/
_INLINE unsigned char __farnspeekb(unsigned long addr) {
	
	if ( !Platform ) { 
		unsigned char result;
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movb (%k1),%b0"
		  : "=q" (result)
		  : "r" (addr));
		return result;
  	} else
		return (*((unsigned char  *)(addr)));
}

/**
	Read an unsigned short at the specified adress
	Works in far memory
	Similar to (*((unsigned short *)(addr))) on windows and linux
	Similar to _farnspeekw on djgpp
	@see __farsetsel
*/
_INLINE unsigned short __farnspeekw(unsigned long addr) {
	
	if ( !Platform ) { 
		unsigned short result;
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movw (%k1),%w0"
		  : "=r" (result)
		  : "r" (addr));
		return result;
	} else
		return (*((unsigned short *)(addr)));
}

/**
	Read an unsigned long at the specified adress
	Works in far memory
	Similar to (*((unsigned long *)(addr))) on windows and linux
	Similar to _farnspeekl on djgpp
	@see __farsetsel
*/
_INLINE unsigned long __farnspeekl(unsigned long addr) {
	
	if ( !Platform ) {
		unsigned long result;
		__asm__ __volatile__ (".byte 0x64\n"
		  "	movl (%k1),%k0"
		  : "=r" (result)
		  : "r" (addr));
		return result;
	} else
		return (*((unsigned long  *)(addr)));
}

/**
@}
*/

/*
	Force allegro to use our functions.
*/

#define _farsetsel(seg) 		__farsetsel(seg)
#define _farnspokeb(addr, val)  __farnspokeb(addr, val)
#define _farnspokew(addr, val)  __farnspokew(addr, val)
#define _farnspokel(addr, val)  __farnspokel(addr, val)
#define _farnspeekb(addr)       __farnspeekb(addr)
#define _farnspeekw(addr)       __farnspeekw(addr)
#define _farnspeekl(addr)       __farnspeekl(addr)
#define __dj_include_sys_farptr_h_

#define ALLEGRO_UNIX

#endif

#endif /* _PLATFORM_H_INCLUDED_ */
