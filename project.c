#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 3
#define TIME_SLICE 1000000

pthread_t threads[NUM_THREADS];
pthread_mutex_t lock;
int current_thread_rr = 0;
int current_thread_mlfq = 0;
int threads_completed = 0;
void sort_threads(int *thread_lengths, int *thread_order) {
    int i, j, temp;
    for (i = 0; i < NUM_THREADS; i++) {
        thread_order[i] = i;
    }
    for (i = 0; i < NUM_THREADS - 1; i++) {
        for (j = i + 1; j < NUM_THREADS; j++) {
            if (thread_lengths[i] > thread_lengths[j]) {
                temp = thread_lengths[i];
                thread_lengths[i] = thread_lengths[j];
                thread_lengths[j] = temp;
                temp = thread_order[i];
                thread_order[i] = thread_order[j];
                thread_order[j] = temp;
            }
        }
    }
}
void *thread_func_rr(void *arg) {
    int id = *(int *) arg;
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == current_thread_rr) {
            printf("Round-robin scheduling: Thread %d is running...\n", id);
            usleep(TIME_SLICE);
            printf("Round-robin scheduling: Thread %d is done.\n", id);
            current_thread_rr = (current_thread_rr + 1) % NUM_THREADS;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_func_ps(void *arg) {
    int id = *(int *) arg;
    int priority = NUM_THREADS - id;
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == current_thread_rr) {
            printf("Priority scheduling: Thread %d is running (priority %d)...\n", id, priority);
            usleep(TIME_SLICE);
            printf("Priority scheduling: Thread %d is done.\n", id);
            current_thread_rr = (current_thread_rr + 1) % NUM_THREADS;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void *thread_func_sjf(void *arg) {
    int id = *(int *) arg;
    int thread_lengths[NUM_THREADS] = {375, 200, 500}; // thread lengths in milliseconds
    int thread_order[NUM_THREADS];
    sort_threads(thread_lengths, thread_order);
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == thread_order[current_thread_rr]) {
            printf("SJF scheduling: Thread %d is running (%d ms)...\n", id, thread_lengths[current_thread_rr]);
            usleep(thread_lengths[current_thread_rr] * 1000);
            printf("SJF scheduling: Thread %d is done.\n", id);
            current_thread_rr++;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void *thread_func_deadline(void *arg) {
    int id = *(int *) arg;
    struct timespec deadline = {0, (id + 1) * 100000000};
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (clock_gettime(CLOCK_REALTIME, &deadline) == -1) {
            perror("clock_gettime");
            exit(EXIT_FAILURE);
        }
        printf("Deadline-based scheduling: Thread %d is running (deadline %lds %ldns)...\n", id, deadline.tv_sec, deadline.tv_nsec);
        usleep(TIME_SLICE);
        printf("Deadline-based scheduling: Thread %d is done.\n", id);
        threads_completed++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void *thread_func_high(void *arg) {
    int id = *(int *) arg;
    int priority = 0;
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == current_thread_mlfq) {
            printf("Multi-level feedback queue scheduling: Thread %d is running (priority %d)...\n", id, priority);
            usleep(TIME_SLICE);
            printf("Multi-level feedback queue scheduling: Thread %d is done.\n", id);
            current_thread_mlfq = (current_thread_mlfq + 1) % NUM_THREADS;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_func_med(void *arg) {
    int id = *(int *) arg;
    int priority = 1;
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == current_thread_mlfq) {
            printf("Multi-level feedback queue scheduling: Thread %d is running (priority %d)...\n", id, priority);
            usleep(TIME_SLICE);
            printf("Multi-level feedback queue scheduling: Thread %d is done.\n", id);
            current_thread_mlfq = (current_thread_mlfq + 1) % NUM_THREADS;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_func_low(void *arg) {
    int id = *(int *) arg;
    int priority = 2;
    while (threads_completed < NUM_THREADS) {
        pthread_mutex_lock(&lock);
        if (id == current_thread_mlfq) {
            printf("Multi-level feedback queue scheduling: Thread %d is running (priority %d)...\n", id, priority);
            usleep(TIME_SLICE);
            printf("Multi-level feedback queue scheduling: Thread %d is done.\n", id);
            current_thread_mlfq = (current_thread_mlfq + 1) % NUM_THREADS;
            threads_completed++;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    int i;
    int thread_ids[NUM_THREADS];
    int scheduling_algorithm;

    pthread_mutex_init(&lock, NULL);
    
                printf("\n\n SIMULATION OF SCHEDULING KERNEL THREADS\n");
		printf("\n Which scheduling algorithm do you want to use?");
		printf("\n 1. ROUND ROBIN.");
		printf("\n 2. PRIORITY.");
		printf("\n 3. SHORTEST JOB FIRST.");
		printf("\n 4. DEADLINE-BASED.");
		printf("\n 5. MULTI-LEVEL FEEDBACK QUEUE.");
		printf("\n Select Option: ");
		printf("\n\n Enter your Choice: ");
                scanf("%d", &scheduling_algorithm);
  
    switch (scheduling_algorithm) 
    {
        case 1:
            for (i = 0; i < NUM_THREADS; i++) 
            {
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, thread_func_rr, &thread_ids[i]);
            }

            for (i = 0; i < NUM_THREADS; i++) 
            {
                pthread_join(threads[i], NULL);
            }
            break;
        case 2:
            for (i = 0; i < NUM_THREADS; i++) 
            {
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, thread_func_ps, &thread_ids[i]);
            }

            for (i = 0; i < NUM_THREADS; i++) 
            {
                pthread_join(threads[i], NULL);
            }
            break;
        case 3:
            for (i = 0; i < NUM_THREADS; i++) {
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, thread_func_sjf, &thread_ids[i]);
            }

            for (i = 0; i < NUM_THREADS; i++) {
                pthread_join(threads[i], NULL);
            }
            break;
        case 4:
            for (i = 0; i < NUM_THREADS; i++) 
            {
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, thread_func_deadline, &thread_ids[i]);
            }

            for (i = 0; i < NUM_THREADS; i++) 
            {
                pthread_join(threads[i], NULL);
            }
            break;
        case 5:
            for (i = 0; i < NUM_THREADS; i++) 
            {
		thread_ids[i] = i;
		if (i < 3) 
		{
		    pthread_create(&threads[i], NULL, thread_func_high, &thread_ids[i]);
		} 
		else if (i < 6) 
		{
		    pthread_create(&threads[i], NULL, thread_func_med, &thread_ids[i]);
		} 
		else 
		{
		    pthread_create(&threads[i], NULL, thread_func_low, &thread_ids[i]);
		}
	    }

    for (i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }
            break;
        default:
            printf("Invalid Input\n");
            break;
    }
    

    pthread_mutex_destroy(&lock);

    return 0;
}
