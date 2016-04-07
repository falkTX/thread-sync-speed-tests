/**
 * thread_sync_latency
 * benchmarks for the latency of various inter-thread synchronization
 * primitives
 *
 * Written in 2013 by William Light <wrl@illest.net>
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

#include <stdlib.h>
#include <time.h>

#include "timeutil.h"
#include "test_params.h"
#include "report.h"

#include "stub_funcs.h"

int
main(int argc, char **argv)
{
	struct timespec start_time, end_time, diff;
	struct thread_sync_impl sync_impl;
	uint64_t i;

	sync_impl_init(&sync_impl);

	clock_gettime(CLOCK_MONOTONIC, &start_time);
	for (i = 0; i < ITERATIONS; i++) {
		sync_impl_signal(&sync_impl);
		sync_impl_wait(&sync_impl);
	}
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	timespec_diff(&diff, &end_time, &start_time);
	report(diff.tv_sec, diff.tv_nsec);

	return EXIT_SUCCESS;
}
