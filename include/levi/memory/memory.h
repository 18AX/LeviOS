#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#define PAGE_SIZE 4096

#include "levi/memory/vmm.h"
#include "levi/types.h"

#define KERNEL_SPACE 0xffffffff80000000

#define HHDM_ADDRESS 0xffff800000000000

#define GIB_4 4294967296
#define KERNEL_SPACE_SIZE 0x80000000

STATUS memory_init(vas_t *kvas);

static inline u64 align_up(u64 ptr)
{
    return (ptr + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

static inline u64 align_down(u64 ptr)
{
    return ptr & ~(PAGE_SIZE - 1);
}

static inline u32 is_align(u64 ptr)
{
    return (ptr & (PAGE_SIZE - 1)) == 0;
}

static inline u64 hhdm_to_phys(u64 address)
{
    return address - HHDM_ADDRESS;
}

static inline u64 phys_to_hhdm(u64 address)
{
    return address + HHDM_ADDRESS;
}

static inline u64 kernel_to_phys(u64 address)
{
    return address - KERNEL_SPACE;
}

static inline u64 phys_to_kernel(u64 address)
{
    return address + KERNEL_SPACE;
}

vas_t kvas(void);

/** \defgroup ALLOCHOOKS liballoc hooks
 *
 * These are the OS specific functions which need to
 * be implemented on any platform that the library
 * is expected to work on.
 */

/** @{ */

// If we are told to not define our own size_t, then we skip the define.
//#define _HAVE_UINTPTR_T
// typedef	unsigned long	uintptr_t;

// This lets you prefix malloc and friends
#define PREFIX(func) k##func

#ifdef __cplusplus
extern "C"
{
#endif

    /** This function is supposed to lock the memory data structures. It
     * could be as simple as disabling interrupts or acquiring a spinlock.
     * It's up to you to decide.
     *
     * \return 0 if the lock was acquired successfully. Anything else is
     * failure.
     */
    int liballoc_lock();

    /** This function unlocks what was previously locked by the liballoc_lock
     * function.  If it disabled interrupts, it enables interrupts. If it
     * had acquiried a spinlock, it releases the spinlock. etc.
     *
     * \return 0 if the lock was successfully released.
     */
    int liballoc_unlock();

    /** This is the hook into the local system which allocates pages. It
     * accepts an integer parameter which is the number of pages
     * required.  The page size was set up in the liballoc_init function.
     *
     * \return NULL if the pages were not allocated.
     * \return A pointer to the allocated memory.
     */
    void *liballoc_alloc(u64 pages);

    /** This frees previously allocated memory. The void* parameter passed
     * to the function is the exact same value returned from a previous
     * liballoc_alloc call.
     *
     * The integer value is the number of pages to free.
     *
     * \return 0 if the memory was successfully freed.
     */
    int liballoc_free(void *ptr, u64 pages);

    void *PREFIX(malloc)(u64); ///< The standard function.
    void *PREFIX(realloc)(void *, u64); ///< The standard function.
    void *PREFIX(calloc)(u64, u64); ///< The standard function.
    void PREFIX(free)(void *); ///< The standard function.

    void liballoc_dump();

#ifdef __cplusplus
}
#endif

/** @} */

#endif