/**
 * thread_sync_latency
 * benchmarks for the latency of various inter-thread synchronization
 * primitives
 *
 * Written in 2015 by Andrew Kelley <superjoe30@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <linux/futex.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/errno.h>
#include "stub_funcs.h"

struct thread_sync_impl {
    int n;
};

static inline int futex_wait(int *address) {
    for (;;)
    {
        if (__sync_bool_compare_and_swap(address, 1, 0))
            return 1;

        if (syscall(__NR_futex, address, FUTEX_WAIT, 0, NULL, NULL, 0) != 0 && errno != EWOULDBLOCK)
            return 0;
    }
}

static inline void futex_wake(int *address) {
    if (! __sync_bool_compare_and_swap(address, 0, 1))
        // already unlocked, do not wake futex
        return;

    syscall(__NR_futex, address, FUTEX_WAKE, 1, NULL, NULL, 0);
}

static int
sync_impl_init(struct thread_sync_impl *impl)
{
    impl->n = 0;
    return 0;
}

static int
sync_impl_wait(struct thread_sync_impl *impl)
{
    futex_wait(&impl->n);
    return 0;
}

static int
sync_impl_signal(struct thread_sync_impl *impl)
{
    futex_wake(&impl->n);
    return 0;
}

#include "stub.inc.c"
