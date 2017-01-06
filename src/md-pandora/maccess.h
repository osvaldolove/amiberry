/*
 * UAE - The Un*x Amiga Emulator
 *
 * Memory access functions
 *
 * Copyright 1996 Bernd Schmidt
 */

#ifndef MACCESS_UAE_H
#define MACCESS_UAE_H

#define ALIGN_POINTER_TO32(p) ((~(unsigned long)(p)) & 3)

STATIC_INLINE uae_u32 do_get_mem_long(uae_u32 *a)
{
#ifdef ARMV6_ASSEMBLY
	uae_u32 v;
	__asm__(
		"ldr %[v], [%[a]] \n\t"
		"rev %[v], %[v] \n\t"
		: [v] "=r" (v) : [a] "r" (a));
	return v;
#else
	uae_u8 *b = (uae_u8 *)a;
	return (*b << 24) | (*(b + 1) << 16) | (*(b + 2) << 8) | (*(b + 3));
#endif
}

STATIC_INLINE uae_u16 do_get_mem_word(uae_u16 *a)
{
#ifdef ARMV6_ASSEMBLY
    uae_u16 v;
    __asm__ (
        "ldrh %[v], [%[a]] \n\t"
        "rev16 %[v], %[v] \n\t"
        : [v] "=r" (v) : [a] "r" (a) );
    return v;
#else
	uae_u8 *b = (uae_u8 *)a;
	return (*b << 8) | (*(b + 1));
#endif
}

#define do_get_mem_byte(a) ((uae_u32)*(uae_u8 *)(a))

STATIC_INLINE void do_put_mem_long(uae_u32 *a, uae_u32 v)
{
#ifdef ARMV6_ASSEMBLY
	__asm__(
		"rev r2, %[v] \n\t"
		"str r2, [%[a]] \n\t"
		: : [v] "r" (v), [a] "r" (a) : "r2", "memory");
#else
	uae_u8 *b = (uae_u8 *)a;

	*b = v >> 24;
	*(b + 1) = v >> 16;
	*(b + 2) = v >> 8;
	*(b + 3) = v;
#endif
}

STATIC_INLINE void do_put_mem_word(uae_u16 *a, uae_u16 v)
{
#ifdef ARMV6_ASSEMBLY
    __asm__ (
        "rev16 r2, %[v] \n\t"
        "strh r2, [%[a]] \n\t"
        : : [v] "r" (v), [a] "r" (a) : "r2", "memory" );

#else
	uae_u8 *b = (uae_u8 *)a;

	*b = v >> 8;
	*(b + 1) = v;
#endif
}

STATIC_INLINE void do_put_mem_byte(uae_u8 *a, uae_u8 v)
{
    *a = v;
}

#define call_mem_get_func(func, addr) ((*func)(addr))
#define call_mem_put_func(func, addr, v) ((*func)(addr, v))

#endif
