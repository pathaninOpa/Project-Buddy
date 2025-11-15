#if !defined(_ZEGOATOMIC_INC_)
#define _ZEGOATOMIC_INC_
#pragma once

#include "zegotypes.h"

/*noly support no barrier mode*/
typedef int32	atomic32;

/*** part of following code comes from chromium ***
 
// Atomically execute:
//      result = *ptr;
//      if (*ptr == old_value)
//        *ptr = new_value;
//      return result;
//
// I.e., replace "*ptr" with "new_value" if "*ptr" used to be "old_value".
// Always return the old value of "*ptr"
//
// This routine implies no memory barriers.
//>>> zegoatomic_exchnage_compare

// Atomically store new_value into *ptr, returning the previous value held in
// *ptr.  This routine implies no memory barriers.
//>>> zegoatomic_increment

// Atomically increment *ptr by "increment".  Returns the new value of
// *ptr with the increment applied.  This routine implies no memory barriers.
//>>> zegoatomic_exchange
 */

#if _OS_WIN_
#	include <windows.h>
#	define	zegoatomic_increment(ptr, increment)							(InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(increment)) + increment)
#	define	zegoatomic_exchnage_compare(ptr, old_value, new_value)		static_cast<atomic32>(InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(new_value), static_cast<LONG>(old_value)))
#	define	zegoatomic_exchange(ptr, new_value)							static_cast<atomic32>(InterlockedExchange(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(new_value)))

#elif defined(_OS_IOS_) || defined(_OS_MAC_)
#	include <libkern/OSAtomic.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

inline atomic32 zegoatomic_increment(volatile atomic32* ptr, atomic32 increment) {
    return OSAtomicAdd32(increment, ptr);
}

inline atomic32 zegoatomic_exchnage_compare(volatile atomic32 *ptr,
                                         atomic32 old_value,
                                         atomic32 new_value) {
	atomic32 prev_value;
	do {
		if (OSAtomicCompareAndSwap32(old_value, new_value,
									 ptr)) {
			return old_value;
		}
		prev_value = *ptr;
	} while (prev_value == old_value);
	return prev_value;
}

inline atomic32 zegoatomic_exchange(volatile atomic32 *ptr,
                                         atomic32 new_value) {
	atomic32 old_value;
	do {
		old_value = *ptr;
	} while (!OSAtomicCompareAndSwap32(old_value, new_value,
									   ptr));
	return old_value;
}

#pragma GCC diagnostic pop

#elif ( defined(_ARM_) && defined(_OS_LINUX_) ) || defined(_OS_ANDROID_) || defined(_OS_OHOS_)
// 0xffff0fc0 is the hard coded address of a function provided by
// the kernel which implements an atomic compare-exchange. On older
// ARM architecture revisions (pre-v6) this may be implemented using
// a syscall. This address is stable, and in active use (hard coded)
// by at least glibc-2.7 and the Android C library.
//typedef atomic32 (*LinuxKernelCmpxchgFunc)(atomic32 old_value,
//                                           atomic32 new_value,
//                                           volatile atomic32* ptr);
//LinuxKernelCmpxchgFunc pLinuxKernelCmpxchg __attribute__((weak)) =
//(LinuxKernelCmpxchgFunc) 0xffff0fc0;
//
//typedef void (*LinuxKernelMemoryBarrierFunc)(void);
//LinuxKernelMemoryBarrierFunc pLinuxKernelMemoryBarrier __attribute__((weak)) =
//(LinuxKernelMemoryBarrierFunc) 0xffff0fa0;

inline atomic32 zegoatomic_exchnage_compare(volatile atomic32* ptr,
                                         atomic32 old_value,
                                         atomic32 new_value) {
	atomic32 prev_value = *ptr;
	do {
        if (__sync_bool_compare_and_swap(const_cast<atomic32*>(ptr), old_value, new_value)) {
//		if (!pLinuxKernelCmpxchg(old_value, new_value,
//								 const_cast<atomic32*>(ptr))) {
			return old_value;
		}
		prev_value = *ptr;
	} while (prev_value == old_value);
	return prev_value;
}

inline atomic32 zegoatomic_exchange(volatile atomic32* ptr,
                                         atomic32 new_value) {
	atomic32 old_value;
	do {
		old_value = *ptr;
//	} while (pLinuxKernelCmpxchg(old_value, new_value,
//								 const_cast<atomic32*>(ptr)));

    } while (!__sync_bool_compare_and_swap(const_cast<atomic32*>(ptr), old_value, new_value));
	return old_value;
}

inline atomic32 zegoatomic_increment(volatile atomic32* ptr,
                                        atomic32 increment) {
	for (;;) {
		// Atomic exchange the old value with an incremented one.
		atomic32 old_value = *ptr;
		atomic32 new_value = old_value + increment;
        if (__sync_bool_compare_and_swap(const_cast<atomic32*>(ptr), old_value, new_value)) {
//		if (pLinuxKernelCmpxchg(old_value, new_value,
//								const_cast<atomic32*>(ptr)) == 0) {
			// The exchange took place as expected.
			return new_value;
		}
		// Otherwise, *ptr changed mid-loop and we need to retry.
	}
}

#else

// 32-bit low-level operations on any platform.

inline atomic32 zegoatomic_exchnage_compare(volatile atomic32* ptr,
                                         atomic32 old_value,
                                         atomic32 new_value) {
	atomic32 prev;
	__asm__ __volatile__("lock; cmpxchgl %1,%2"
						 : "=a" (prev)
						 : "q" (new_value), "m" (*ptr), "0" (old_value)
						 : "memory");
	return prev;
}

inline atomic32 zegoatomic_exchange(volatile atomic32* ptr,
                                         atomic32 new_value) {
	__asm__ __volatile__("xchgl %1,%0"  // The lock prefix is implicit for xchg.
						 : "=r" (new_value)
						 : "m" (*ptr), "0" (new_value)
						 : "memory");
	return new_value;  // Now it's the previous value.
}

inline atomic32 zegoatomic_increment(volatile atomic32* ptr,
                                          atomic32 increment) {
	atomic32 temp = increment;
	__asm__ __volatile__("lock; xaddl %0,%1"
						 : "+r" (temp), "+m" (*ptr)
						 : : "memory");
	// temp now holds the old value of *ptr
	return temp + increment;
}

#endif

#endif /*_ZEGOATOMIC_INC_*/
