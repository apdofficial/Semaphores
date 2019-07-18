/*******************************************************************************
 *                                                                             *
 *   Program    : Semaphores                                                   *
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

struct Queues {
    int *singleQueue;
    int *normalQueue;
    int singleTotal;
    int normalTotal;
};

struct Queues queues;

sem_t MutexPeople;
sem_t MutexRollerCoaster;

int freeSeats;
bool needPeople = false;

int singleRiderControl = 0;
bool singleRiderAdded = false;

bool notFull = true;
bool singleRiderMode = false;

/*
 * Function: tSingleQueue
 * ----------------------------
 *   Producer
 *
 *   Produces people into the single person queues every 2 seconds.
 *   returns: void
 */
void *tSingleQueue(void *ptr) {
    printf(">  THREAD: tSingleQueue        launches!\n");
    while (true) {
        if (!needPeople)
            sem_wait(&MutexPeople);
        vector_push_back(queues.singleQueue, 1);
        sem_post(&MutexRollerCoaster);
        queues.singleTotal += 1;
        sleep(SINGLEQUEUE_SPEED);
    }
    pthread_exit(0);
}

/*
 * Function: tnormalQueue
 * ----------------------------
 *   Producer
 *
 *   Produces people into the group of 2/3/4/5 person queues every 1 seconds.
 *   returns: void
 */
void *tnormalQueue(void *ptr) {
    printf(">  THREAD: tnormalQueue        launches!\n");
    while (true) {
        if (!needPeople)
            sem_wait(&MutexPeople);
        int groups = rand() % 2 + 2 + rand() % 2 + rand() % 2;
        vector_push_back(queues.normalQueue, groups);
        sem_post(&MutexRollerCoaster);
        queues.normalTotal += groups;
        sleep(NORMALQUEUE_SPEED);
    }
    pthread_exit(0);
}

/*
 * Function: pop_Queue
 * ----------------------------
 *   Pops last person from the Queues type based on the type if 1 normalQueue if 0 singleQueue
 *   returns: true
 */
bool pop_Queue(const int *item, const int type) {
    freeSeats -= *item;
    printf("  |%d|", *item);
    if (type == 1) {
        vector_pop_back(queues.normalQueue);
        queues.normalTotal -= *item;
    } else if (type == 2) {
        vector_pop_back(queues.singleQueue);
        queues.singleTotal -= *item;
        singleRiderAdded = true;
    }
    return true;
}

/*
 * Function: fillRollerCoaster
 * ----------------------------
 *   Fills in the Roller/Dive coaster.
 *   returns: true
 */
bool fillRollerCoaster(void) {
        if(vector_empty(queues.normalQueue) == 0) {
            if (singleRiderMode) {
                if ((freeSeats - *vector_end(queues.normalQueue, 1)) >= 0)
                    pop_Queue(vector_end(queues.normalQueue, 1),1);
                while (freeSeats - *vector_end(queues.singleQueue, 1) >= 0 ) {
                    if(*vector_end(queues.singleQueue, 1) == 0)
                        break;
                    pop_Queue(vector_end(queues.singleQueue, 1),2);
                }
                singleRiderControl = 0;
                singleRiderMode = false;
                return true;
            }
            if ((freeSeats - *vector_end(queues.normalQueue, 1)) >= 0) {
                return pop_Queue(vector_end(queues.normalQueue, 1), 1);
            } else if (((*vector_end(queues.normalQueue, 1) == 3) && (freeSeats - 2 == 0)) ||
                       (*vector_end(queues.normalQueue, 1) == 2 && *vector_end(queues.normalQueue, 2) == 2)) {
                int i = 0;
                for (int *it = vector_begin(queues.normalQueue); it != vector_end(queues.normalQueue, 0); ++it) {
                    if (*it == 2) {
                        freeSeats -= 2;
                        printf("  |%d|", *vector_end(queues.normalQueue, 1));
                        vector_erase(queues.normalQueue, i);
                        return true;
                    }
                    ++i;
                }
            }
        }
        if(vector_empty(queues.singleQueue) == 0) {
            if ((*vector_end(queues.singleQueue, 1) > 0 && (freeSeats - *vector_end(queues.singleQueue, 1)) >= 0)) {
                    return pop_Queue(vector_end(queues.singleQueue, 1), 2);
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
        needPeople = false;
        notFull = true;
        freeSeats = MAX_PEOPLE_LIMIT;
        printf("\n\n~~~~~~~~~~~~~Roller/Dive Coaster~~~~~~~~~~~~~\n");
        printf("         ");
        while (notFull) {
            notFull = fillRollerCoaster();
            if (freeSeats == 0){
                notFull = false;
            } else {
                if (vector_empty(queues.singleQueue) != 0 && vector_empty(queues.normalQueue) != 0 && freeSeats < 6) {
                    notFull = false;
                    needPeople = true;
                    sem_post(&MutexPeople);
                    usleep(50 * 1000);
                }else if (vector_empty(queues.singleQueue) != 0 && vector_empty(queues.normalQueue) != 0 && freeSeats == MAX_PEOPLE_LIMIT) {
                    needPeople = true;
                    sem_post(&MutexPeople);
                    usleep(50 * 1000);
                    sem_wait(&MutexRollerCoaster);
                }else if (vector_empty(queues.singleQueue) != 0 && freeSeats - *vector_end(queues.normalQueue,1) < 0) {
                    notFull = false;
                }
                usleep(1 * 1000);
            }
        }
        singleRiderControl++;
        if (singleRiderAdded) {
            singleRiderControl--;
            singleRiderAdded = false;
        }
        if (singleRiderControl > 2) {
            singleRiderMode = true;
        }
        printf("\n>  ride nr                      : %d\n", rideNr);
        printf(">  number of free seats     left: %d\n", freeSeats);
        printf(">  number of normalQueue tickets: %d\n", queues.normalTotal);
        printf(">  number of singleQueue tickets: %d\n", queues.singleTotal);
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