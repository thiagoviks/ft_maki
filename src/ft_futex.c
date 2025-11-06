#include "../include/ft_maki.h"

/*
** ============================================================================
** FUTEX SYSCALL WRAPPER
** ============================================================================
*/

/*
** futex - Fast userspace locking primitive
** @uaddr: address of futex word
** @op: operation (WAIT, WAKE, etc.)
** @val: expected value for WAIT, number of waiters to wake for WAKE
** @timeout: timeout for WAIT operations (NULL = infinite)
**
** Returns: 0 on success, -1 on error (check ft_errno)
*/
static long ft_futex(int *uaddr, int op, int val,
                     const struct ft_timespec *timeout) {
  return ft_syscall(FT_SYS_FUTEX, uaddr, op, val, timeout, FT_NULL, 0);
}

/*
** ============================================================================
** ATOMIC OPERATIONS
** ============================================================================
** Using GCC built-in atomic operations (compile to CPU instructions)
** These are NOT libc calls - they're compiler intrinsics
*/

/* Atomic compare-and-swap: if *ptr == old, set *ptr = new, return old value */
#define ft_atomic_cas(ptr, old, new) __sync_val_compare_and_swap(ptr, old, new)

/* Atomic fetch-and-add: atomically add val to *ptr, return old value */
#define ft_atomic_fetch_add(ptr, val) __sync_fetch_and_add(ptr, val)

/* Atomic fetch-and-sub: atomically subtract val from *ptr, return old value */
#define ft_atomic_fetch_sub(ptr, val) __sync_fetch_and_sub(ptr, val)

/* Memory barrier - ensure all operations complete before continuing */
#define ft_memory_barrier() __sync_synchronize()

/*
** ============================================================================
** MUTEX IMPLEMENTATION
** ============================================================================
** Mutex states:
**   0 = unlocked
**   1 = locked, no waiters
**   2 = locked, has waiters (need to call futex_wake on unlock)
**
** This is the standard Linux futex mutex algorithm used by glibc.
*/

/*
** ft_mutex_init - Initialize a mutex
** @mutex: pointer to mutex structure
**
** Returns: 0 on success, -1 on error
*/
int ft_mutex_init(ft_mutex_t *mutex) {
  if (!mutex)
    return -1;

  mutex->futex_word = 0;
  return 0;
}

/*
** ft_mutex_lock - Acquire mutex (blocking)
** @mutex: pointer to mutex structure
**
** Algorithm:
** 1. Try to acquire lock (0 -> 1) with atomic CAS
** 2. If successful, we're done (fast path)
** 3. If failed, enter slow path:
**    a. Set state to 2 (has waiters)
**    b. Call futex_wait to sleep until woken
**    c. Retry acquisition
**
** Returns: 0 on success, -1 on error
*/
int ft_mutex_lock(ft_mutex_t *mutex) {
  int c;

  if (!mutex)
    return -1;

  /* Fast path: try to acquire uncontended lock */
  c = ft_atomic_cas(&mutex->futex_word, 0, 1);
  if (c == 0)
    return 0; /* Success! Lock was free */

  /* Slow path: lock is contended, we need to wait */
  do {
    /* If already set to 2 (has waiters), or try to set it to 2 */
    if (c == 2 || ft_atomic_cas(&mutex->futex_word, 1, 2) != 0) {
      /*
      ** Wait in kernel until woken.
      ** We expect the value to be 2. If it's not, futex returns immediately.
      ** This handles the race where lock is released between our CAS and
      * futex_wait.
      */
      ft_futex(&mutex->futex_word, FT_FUTEX_WAIT_PRIVATE, 2, FT_NULL);
    }

    /* Try to acquire lock, setting state to 2 (we might be a waiter) */
  } while ((c = ft_atomic_cas(&mutex->futex_word, 0, 2)) != 0);

  return 0;
}

/*
** ft_mutex_trylock - Try to acquire mutex (non-blocking)
** @mutex: pointer to mutex structure
**
** Returns: 0 on success (lock acquired), -1 if already locked
*/
int ft_mutex_trylock(ft_mutex_t *mutex) {
  if (!mutex)
    return -1;

  /* Try to acquire lock (0 -> 1) */
  if (ft_atomic_cas(&mutex->futex_word, 0, 1) == 0)
    return 0; /* Success */

  return -1; /* Lock already held */
}

/*
** ft_mutex_unlock - Release mutex
** @mutex: pointer to mutex structure
**
** Algorithm:
** 1. Atomically decrement lock state
** 2. If old value was 1 (locked, no waiters), we're done
** 3. If old value was 2 (locked with waiters), wake one waiter
**
** Returns: 0 on success, -1 on error
*/
int ft_mutex_unlock(ft_mutex_t *mutex) {
  if (!mutex)
    return -1;

  /* Decrement state. If it was 1 (no waiters), we're done. */
  if (ft_atomic_fetch_sub(&mutex->futex_word, 1) != 1) {
    /* There were waiters (state was 2), need to wake one */
    mutex->futex_word = 0; /* Reset to unlocked */

    /* Wake one waiting thread */
    ft_futex(&mutex->futex_word, FT_FUTEX_WAKE_PRIVATE, 1, FT_NULL);
  }

  return 0;
}

/*
** ft_mutex_destroy - Destroy mutex
** @mutex: pointer to mutex structure
**
** Note: No actual cleanup needed for futex-based mutexes.
** Just a sanity check that mutex is unlocked.
**
** Returns: 0 on success, -1 if mutex is still locked
*/
int ft_mutex_destroy(ft_mutex_t *mutex) {
  if (!mutex)
    return -1;

  /* Warn if destroying a locked mutex */
  if (mutex->futex_word != 0) {
    ft_dprintf(FT_STDERR_FILENO,
               "Warning: destroying locked mutex (state=%d)\n",
               mutex->futex_word);
    return -1;
  }

  return 0;
}
