/*******************************************************************************
 *                                                                             *
 *   Program    : Semaphores                                                  *
 *   Programmer : Andrej Pistek (450966)                                       *
 *   Date       : 05-July-2018                                                 *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_PEOPLE_LIMIT    6
#define ROLLERCOASTER_SPEED 1
#define SINGLEQUEUE_SPEED   2
#define NORMALQUEUE_SPEED   1

sem_t MutexPeople;
sem_t MutexRollerCoaster;

int *singleQueue;
int *normalQueue;

int normalTotal = 0;
int singleTotal = 0;

int freeSeats;
bool needPeople = false;

int singleRiderControl = 0;
bool singleRiderAdded = false;

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
        if (!needPeople)
            sem_wait(&MutexPeople);
        vector_push_back(singleQueue, 1);
        sem_post(&MutexRollerCoaster);
        singleTotal += 1;
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
        if (!needPeople)
            sem_wait(&MutexPeople);
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
 *   Pops last person from the Queue type based of the type if 1 normalQueue if 0 singleQueue
 *   returns: true
 */
bool pop_Queue(const int *lastItemInQueue, const int type) {
    freeSeats -= *lastItemInQueue;
    printf("  |%d|", *lastItemInQueue);
    if (type == 1) {
        vector_pop_back(normalQueue);
        normalTotal -= *lastItemInQueue;
    } else if (type == 2) {
        vector_pop_back(singleQueue);
        singleTotal -= *lastItemInQueue;
        singleRiderAdded = true;
    }
    return true;
}

/*
 * Function: fillRollerCoaster
 * ----------------------------
 *   Fills in the Roller/Dive coaster.
 *   returns: true if Roller/Dive coaster is not full, false otherwise
 */
bool fillRollerCoaster(void) {
    if (vector_empty(singleQueue) == 0 || vector_empty(normalQueue) == 0) {
        if (fireSingleRiderMode) {
            if ((freeSeats - *vector_end(normalQueue, 1)) >= 0)
                pop_Queue(vector_end(normalQueue, 1),1);
            while (freeSeats - *vector_end(singleQueue, 1) >= 0 && *vector_end(normalQueue, 1) > 0 ) {
                pop_Queue(vector_end(singleQueue, 1),2);
            }
            singleRiderControl = 0;
            fireSingleRiderMode = false;
            return true;
        }

        if(vector_empty(normalQueue) == 0) {
            if ((freeSeats - *vector_end(normalQueue, 1)) >= 0) {
                return pop_Queue(vector_end(normalQueue, 1), 1);
            } else if (((*vector_end(normalQueue, 1) == 3) && (freeSeats - 2 == 0)) ||
                       (*vector_end(normalQueue, 1) == 2 && *vector_end(normalQueue, 2) == 2)) {
                int i = 0;
                for (int *it = vector_begin(normalQueue); it != vector_end(normalQueue, 0); ++it) {
                    if (*it == 2) {
                        freeSeats -= 2;
                        printf("  |%d|", *vector_end(normalQueue, 1));
                        vector_erase(normalQueue, i);
                        return true;
                    }
                    ++i;
                }
            }
        }
        if(vector_empty(singleQueue) == 0) {
            if ((vector_size(normalQueue) == 0 && (freeSeats - *vector_end(singleQueue, 1)) >= 0) ||
                (freeSeats - *vector_end(singleQueue, 1)) >= 0) {
                return pop_Queue(vector_end(singleQueue, 1), 2);
            }
        }
    }
    return true;
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
        freeSeats = MAX_PEOPLE_LIMIT;
        printf("\n\n~~~~~~~~~~~~~Roller/Dive Coaster~~~~~~~~~~~~~\n");
        printf("         ");
        while (notFull) {
            notFull = fillRollerCoaster();
            if (freeSeats == 0){
                notFull = false;
            } else {
                if (vector_empty(singleQueue) != 0 && vector_empty(normalQueue) != 0 && freeSeats < 6) {
                    notFull = false;
                    needPeople = true;
                    sem_post(&MutexPeople);
                    usleep(50 * 1000);
                }else if (vector_empty(singleQueue) != 0 && vector_empty(normalQueue) != 0 && freeSeats == MAX_PEOPLE_LIMIT) {
                    needPeople = true;
                    sem_post(&MutexPeople);
                    usleep(50 * 1000);
                    sem_wait(&MutexRollerCoaster);
                }
                usleep(5 * 1000);
            }
        }
        singleRiderControl++;
        if (singleRiderAdded) {
            singleRiderControl--;
            singleRiderAdded = false;
        }
        if (singleRiderControl > 2) {
            fireSingleRiderMode = true;
        }
        printf("\n>  ride nr                      : %d\n", rideNr);
        printf(">  number of free seats     left: %d\n", freeSeats);
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
    int targ[3];
    pthread_t thread[3];

    sem_init(&MutexPeople, 0, 1);
    sem_init(&MutexRollerCoaster, 0, 1);

    pthread_create(&thread[0], NULL, &tnormalQueue, (void *) &targ[0]);
    pthread_create(&thread[1], NULL, &tSingleQueue, (void *) &targ[1]);
    pthread_create(&thread[2], NULL, &tRollerCoaster, (void *) &targ[2]);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    sem_destroy(&MutexRollerCoaster);
    sem_destroy(&MutexPeople);

    return 0;
}