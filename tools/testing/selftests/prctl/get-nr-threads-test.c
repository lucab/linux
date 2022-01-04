// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Test for prctl(PR_GET_NR_THREADS, ...)
 *
 * Basic test to check the behaviour of PR_GET_NR_THREADS
 */

#include <assert.h>
//#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/prctl.h>
#include <linux/prctl.h>

#ifndef PR_GET_NR_THREADS
#define PR_GET_NR_THREADS 63
#endif

void assert_nr_threads(unsigned int expected)
{
	unsigned int num_threads = 0;

	printf("prctl(PR_GET_NR_THREADS, ...)\n");
	if (prctl(PR_GET_NR_THREADS, &num_threads) == -1) {
		perror("prctl");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}
	printf("num_threads == %d\n", num_threads);
	assert(num_threads == expected);
	return;
}

void *threaded_assert(void *input)
{
	assert_nr_threads(2);
	return NULL;
}

int main(void)
{
	int r;
	pthread_t tinfo;

	assert_nr_threads(1);

	r = pthread_create(&tinfo, NULL, threaded_assert, NULL);
	assert(r == 0);

	r = pthread_join(tinfo, NULL);
	assert_nr_threads(1);

	fflush(stdout);
	return EXIT_SUCCESS;
}
