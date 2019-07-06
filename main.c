#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

sem_t MutexPeople;
sem_t MutexRollercoaster;

int *singleQueue;
int *normalQueue;

int groupTotal = 0;
int singleTotal = 0;

int rollerCoaster;
bool ready = false;

/*
 * Function: tsingleQueue
 * ----------------------------
 *   Produces people into the single person queue every 2 seconds.
 *   returns: void
 */
void *tsingleQueue(void *ptr) {
    printf(">  THREAD: tsingleQueue        launches!\n");
    while (true) {
        if (!ready) {
            sem_wait(&MutexPeople);
        }
        vector_push_back(singleQueue, 1);
        sem_post(&MutexRollercoaster);
        sleep(2);
    }
    pthread_exit(0);
}

/*
 * Function: tgroupqueue
 * ----------------------------
 *   Produces people into the group person queue every 1 seconds.
 *   returns: void
 */
void *tgroupqueue(void *ptr) {
    printf(">  THREAD: tgroupqueue         launches!\n");
    while (true) {
        if (!ready) {
            sem_wait(&MutexPeople);
        }
        int groups = rand() % 2 + 2;
        vector_push_back(normalQueue, groups);
        sem_post(&MutexRollercoaster);
        groupTotal += groups;
        sleep(1);
    }
    pthread_exit(0);
}

bool fillRollercoaster(void) {
    int *lastSingleQueue = vector_end(singleQueue);
    int *lastNormalQueue = vector_end(normalQueue);

    if (*lastSingleQueue > 0 && *lastNormalQueue > 0) {
        if (vector_empty(normalQueue) == 0 && vector_empty(singleQueue) != 0 &&
            (rollerCoaster - *lastSingleQueue) >= 0) {
            rollerCoaster -= *lastSingleQueue;
            printf("  |%d|", *lastSingleQueue);
            vector_pop_back(singleQueue);
            return true;
        } else if ((rollerCoaster - *lastNormalQueue) >= 0) {
            rollerCoaster -= *lastNormalQueue;
            groupTotal -= *lastNormalQueue;
            printf("  |%d|", *lastNormalQueue);
            vector_pop_back(normalQueue);
            return true;
        } else if ((*lastNormalQueue == 3) && (rollerCoaster - 2 == 0)) {
            int *it;
            int i = 0;
            for (it = vector_begin(singleQueue); it != vector_end(singleQueue); ++it) {
                if (*it == 2) {
                    rollerCoaster -= 2;
                    printf("  |%d|",*lastNormalQueue);
                    vector_erase(normalQueue, i);
                    return true;
                }
                ++i;
            }
        } else if ((rollerCoaster - *lastSingleQueue) >= 0) {
            rollerCoaster -= *lastSingleQueue;
            printf("  |%d|", *lastSingleQueue);
            vector_pop_back(singleQueue);
            return true;
        }
    }
    return false;
}

/*
 * Function: tgroupqueue
 * ----------------------------
 *   Takes people into the Roller/Dive coaster  every 5 seconds.
 *   It has one row where can be fitted 6 people.
 *   returns: void
 */
void *trollercoaster(void *ptr) {
    printf(">  THREAD: trollercoaster      launches!\n");
    int rideNr =0;
    while (true) {
        ready = false;
        bool filling = true;
        rollerCoaster = 6;

        printf("\n\n~~~~~~~~~~~Roller/Dive Coaster~~~~~~~~~~~\n");
        printf(    "         ");
        while (filling) {
            if (vector_size(singleQueue) == 0 && vector_size(normalQueue) == 0) {
                sleep(1);
                sem_wait(&MutexRollercoaster);
            } else {

                filling = fillRollercoaster();

                if (rollerCoaster == 6) {
                    sem_post(&MutexPeople);
                    filling = true;
                }
            }
        }
        if (singleQueue) {
            size_t i;
            for (i = 0; i < vector_size(singleQueue); ++i) {
                singleTotal += singleQueue[i];
            }
        }
        printf("\n>  ride nr                      : %d\n",rideNr);
        printf(">  number of free seats     left: %d\n", rollerCoaster);
        printf(">  number of groupQueue  tickets: %d\n", groupTotal);
        printf(">  number of singleQueue tickets: %d\n", singleTotal);
        sem_post(&MutexPeople);
        printf("~~~~~~~~Rollercoaster departed!~~~~~~~~~~\n\n");
        ready = true;
        rideNr++;
        sleep(5);
    }
    pthread_exit(0);
}

/*
 * Function: main
 * ----------------------------
 *   Initializes all the threads and runs the program
 *   returns: 0
 */
int main() {
    printf("\n"
           "*************************************************\n"
           "* Wellcome to the Roller/Dive coaster which has *\n"
           "* 6 seats and departs every 5 seconds.          *\n"
           "* Student: Andrej Pištek                        *\n"
           "* Student number: 450966                        *\n"
           "*************************************************\n\n");

    int targ[3];
    pthread_t thread[3];

    sem_init(&MutexPeople, 0, 1);
    sem_init(&MutexRollercoaster, 0, 1);

    pthread_create(&thread[0], NULL, &tgroupqueue, (void *) &targ[0]);
    pthread_create(&thread[1], NULL, &tsingleQueue, (void *) &targ[1]);
    pthread_create(&thread[2], NULL, &trollercoaster, (void *) &targ[2]);

    sem_post(&MutexPeople);
    sem_wait(&MutexRollercoaster);


    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    sem_destroy(&MutexRollercoaster);
    sem_destroy(&MutexPeople);

    return 0;
}