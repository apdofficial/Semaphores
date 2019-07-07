#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_PEOPLE_LIMIT    6
#define ROLLERCOASTER_SPEED 2
#define SINGLEQUEUE_SPEED   2
#define NORMALQUEUE_SPEED   1

sem_t MutexPeople;
sem_t MutexRollerCoaster;

int *singleQueue;
int *normalQueue;

int normalTotal = 0;
int singleTotal = 0;

int rollerCoaster;
bool needPeople = false;

int singleRiderControl = 0;
bool singleRiderAdded = true;

bool notFull = true;
bool fireSingleRiderMode = false;

/*
 * Function: tSingleQueue
 * ----------------------------
 *   Producer
 *
 *   Produces people into the single person queue every 2 seconds.
 *   returns: void
 */
void *tSingleQueue(void *ptr) {
    printf(">  THREAD: tSingleQueue        launches!\n");
    while (true) {
        if (!needPeople) {
            sem_wait(&MutexPeople);
        }
        vector_push_back(singleQueue, 1);
        sem_post(&MutexRollerCoaster);
        sleep(SINGLEQUEUE_SPEED);
    }
    pthread_exit(0);
}

/*
 * Function: tnormalQueue
 * ----------------------------
 *   Producer
 *
 *   Produces people into the group of 2/3 person queue every 1 seconds.
 *   returns: void
 */
void *tnormalQueue(void *ptr) {
    printf(">  THREAD: tnormalQueue        launches!\n");
    while (true) {
        if (!needPeople) {
            sem_wait(&MutexPeople);
        }
        int groups = rand() % 2 + 2;
        vector_push_back(normalQueue, groups);
        sem_post(&MutexRollerCoaster);
        normalTotal += groups;
        sleep(NORMALQUEUE_SPEED);
    }
    pthread_exit(0);
}

/*
 * Function: fillRollerCoaster
 * ----------------------------
 *   Fills in the Roller/Dive coaster.
 *   returns: true if Roller/Dive coaster is not full, false otherwise
 */
bool fillRollerCoaster(void) {
    if (singleQueue && normalQueue) {
        int *lastSingleQueue = vector_end(singleQueue, 1);
        int *lastNormalQueue = vector_end(normalQueue, 1);

        if (*lastSingleQueue > 0 && *lastNormalQueue > 0) {

            if (fireSingleRiderMode) {
                printf("\nsingleRiderAdded fired!\n");
                if ((rollerCoaster - *lastNormalQueue) >= 0) {
                    rollerCoaster -= *lastNormalQueue;
                    normalTotal -= *lastNormalQueue;
                    printf("  |%d|", *lastNormalQueue);
                    vector_pop_back(normalQueue);
                }
                while (notFull) {
                    if ((rollerCoaster - *vector_end(singleQueue, 1)) >= 0) {
                        rollerCoaster -= *lastSingleQueue;
                        printf("  |%d|", *lastSingleQueue);
                        vector_pop_back(singleQueue);
                        singleRiderAdded = true;
                        notFull= true;
                    } else {
                        notFull= false;
                    }
                }
                singleRiderControl =0;
                fireSingleRiderMode =false;
                return true;
            }

            if (vector_empty(normalQueue) == 0 && vector_empty(singleQueue) != 0 &&
                (rollerCoaster - *lastSingleQueue) >= 0) {
                rollerCoaster -= *lastSingleQueue;
                printf("  |%d|", *lastSingleQueue);
                vector_pop_back(singleQueue);
                singleRiderAdded = true;
                return true;
            }
            if ((rollerCoaster - *lastNormalQueue) >= 0) {
                rollerCoaster -= *lastNormalQueue;
                normalTotal -= *lastNormalQueue;
                printf("  |%d|", *lastNormalQueue);
                vector_pop_back(normalQueue);
                return true;
            } else if ((*lastNormalQueue == 3) && (rollerCoaster - 2 == 0)) {
                int *it;
                int i = 0;
                for (it = vector_begin(singleQueue); it != vector_end(singleQueue, 0); ++it) {
                    if (*it == 2) {
                        rollerCoaster -= 2;
                        printf("  |%d|", *lastNormalQueue);
                        vector_erase(normalQueue, i);
                        return true;
                    }
                    ++i;
                }
            } else if (*lastNormalQueue == 2 && *vector_end(normalQueue, 2) == 2) {
                int *it;
                int i = 0;
                for (it = vector_begin(singleQueue); it != vector_end(singleQueue, 0); ++it) {
                    if (*it == 2) {
                        rollerCoaster -= 2;
                        printf("  |%d|", *lastNormalQueue);
                        vector_erase(normalQueue, i);
                        notFull = true;
                    }
                    ++i;
                }
            }
            if ((rollerCoaster - *lastSingleQueue) >= 0) {
                rollerCoaster -= *lastSingleQueue;
                printf("  |%d|", *lastSingleQueue);
                vector_pop_back(singleQueue);
                singleRiderAdded = true;
                return true;
            }

            if (vector_size(normalQueue) == 0 && (rollerCoaster - *lastSingleQueue) >= 0) {
                rollerCoaster -= *lastSingleQueue;
                printf("  |%d|", *lastSingleQueue);
                vector_pop_back(singleQueue);
                singleRiderAdded = true;
                return true;
            }
        }
    }
    return false;
}

/*
 * Function: tnormalQueue
 * ----------------------------
 *   Consumer
 *
 *   Takes people into the Roller/Dive coaster  every 5 seconds.
 *   It has one row where can be fitted 6 people.
 *   returns: void
 */
void *tRollerCoaster(void *ptr) {
    printf(">  THREAD: tRollerCoaster      launches!\n");
    int rideNr = 0;
    while (true) {
        needPeople = true;
        notFull = true;
        rollerCoaster = 6;
        printf("\n\n~~~~~~~~~~~~~Roller/Dive Coaster~~~~~~~~~~~~~\n");
        printf("         ");
        while (notFull) {
            if (vector_size(singleQueue) == 0 && vector_size(normalQueue) == 0 && rollerCoaster == 6) {
                needPeople = true;
                sem_post(&MutexPeople);
                sleep(1);
                sem_wait(&MutexRollerCoaster);
            }
            notFull = fillRollerCoaster();

            if (rollerCoaster == MAX_PEOPLE_LIMIT) {
                notFull = true;
                needPeople = true;
                sem_post(&MutexPeople);
            }
            if ((vector_size(singleQueue) == 0 && vector_size(normalQueue) == 0) && rollerCoaster > 0) {
                notFull = false;
            }
        }

        if (singleQueue) {
            size_t i;
            for (i = 0; i < vector_size(singleQueue); ++i) {
                singleTotal += singleQueue[i];
            }
        }

        singleRiderControl++;

        if (singleRiderAdded) {
            singleRiderControl--;
            singleRiderAdded = false;
        }

        if (singleRiderControl > 2) {
            fireSingleRiderMode =true;
        }

        printf("\n>  ride nr                      : %d\n", rideNr);
        printf(">  number of free seats     left: %d\n", rollerCoaster);
        printf(">  number of normalQueue tickets: %d\n", normalTotal);
        printf(">  number of singleQueue tickets: %d\n", singleTotal);
        printf("~~~~~~~~Roller/Dive Coaster departed!~~~~~~~~\n\n");
        needPeople = true;
        sem_post(&MutexPeople);
        rideNr++;
        sleep(ROLLERCOASTER_SPEED);
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
    sem_init(&MutexRollerCoaster, 0, 1);

    pthread_create(&thread[0], NULL, &tnormalQueue, (void *) &targ[0]);
    pthread_create(&thread[1], NULL, &tSingleQueue, (void *) &targ[1]);
    pthread_create(&thread[2], NULL, &tRollerCoaster, (void *) &targ[2]);

    sem_post(&MutexPeople);
    sem_wait(&MutexRollerCoaster);


    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    sem_destroy(&MutexRollerCoaster);
    sem_destroy(&MutexPeople);

    return 0;
}