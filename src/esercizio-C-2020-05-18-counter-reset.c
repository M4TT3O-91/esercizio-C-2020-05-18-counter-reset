/*
 * esercizio-C-2020-05-18-counter-reset.c
 *
 *  Created on: May 19, 2020
 *      Author: marco
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h> //<- include per thread

#define LIMIT 100000
#define RESET_LIMIT 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//variabili condivise
int counter;
int reset;

void common_thread_part() {

	int s;

	while (1) {
		s = pthread_mutex_lock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		counter++;
		if (counter > LIMIT) {
			counter = counter - LIMIT;
			reset++;
		}

		s = pthread_mutex_unlock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		if (reset > RESET_LIMIT)
			return;
	}
}

void* thread_function_1() {
	common_thread_part();

	return NULL;
}

void* thread_function_2() {
	common_thread_part();

	return NULL;
}

int main() {
	pthread_t t1;
		pthread_t t2;
		void * res;

		int s;

		s = pthread_create(&t1, NULL, thread_function_1, NULL);

		if (s != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}

		s = pthread_create(&t2, NULL, thread_function_2, NULL);

		if (s != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}


		s = pthread_join(t1, &res);

		if (s != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}

		s = pthread_join(t2, &res);

		if (s != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}

		printf("final value of shared_counter = %d and reset = %d\n", counter,reset);

		printf("bye\n");

		return 0;
	}
