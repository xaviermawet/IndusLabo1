/* -------------------------------------------------------------------------- *
 *                                     TRI                                    *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/common.h"
#include "../lib/NSignals.h"
#include "../lib/NMessageQueue.h"
#include "../lib/NSharedMemory.h"
#include "../lib/NSemaphore.h"
#include "../hospital.h"

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val);
void handler_sigusr1_managePatient(int sig, siginfo_t* siginfo_handler, int* val);

Semaphore    mutex_shm;
SharedMemory shm;
MessageQueue mq_traitement;

int main(void)
{
    pid_t pid_tri;

    // Free resources
    setSignalHandler(SIGINT, handler_sigint_exit);

    /* ---------------------------------------------------------------------- *
     *                        CREATE SEMAPHORE (MUTEX)                        *
     * ---------------------------------------------------------------------- */
    if (createSemaphore(&mutex_shm, SEM_MUTEX_SHM_NAME, 1)  == 1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Mutex created\n");

    /* ---------------------------------------------------------------------- *
     *                          CREATE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if (createSharedMemory(&shm, SHM_NAME, SHM_NB_RECORDS * sizeof(pid_t)) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory created\n");


    /* ---------------------------------------------------------------------- *
     *                         WRITE IN SHARED MEMORY                         *
     * ---------------------------------------------------------------------- */
    pid_tri = getpid();
    printf("PID = %d\n", pid_tri);

    waitSemaphore(mutex_shm);
    writeSharedMemory(&shm, &pid_tri, sizeof(pid_t), 0);
    signalSemaphore(mutex_shm);

    GREENPRINTF("PID writen\n");

    /* ---------------------------------------------------------------------- *
     *                           CLOSE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if (closeSharedMemory(shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory closed\n");

    /* ---------------------------------------------------------------------- *
     *                         CLOSE SEMAPHORE (MUTEX)                        *
     * ---------------------------------------------------------------------- */
    if (closeSemaphore(mutex_shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Mutex closed\n");

    /* ---------------------------------------------------------------------- *
     *                           OPEN MESSAGE QUEUE                           *
     * ---------------------------------------------------------------------- */
    if (openMessageQueue(&mq_traitement, MQ_NAME) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Message queue opened\n");

    /* ---------------------------------------------------------------------- *
     *                           SET SIGNAL HANDLER                           *
     * ---------------------------------------------------------------------- */
    setSignalHandler(SIGUSR1, handler_sigusr1_managePatient);

    // Loop for receiving signals
    while(TRUE)
        pause();
}

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    /* ---------------------------------------------------------------------- *
     *                           CLOSE MESSAGE QUEUE                          *
     * ---------------------------------------------------------------------- */
    if (closeMessageQueue(&mq_traitement) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("\nMesssage queue closed\n");

    /* ---------------------------------------------------------------------- *
     *                          DESTROY SHARED MEMORY                         *
     * ---------------------------------------------------------------------- */
    if (destroySharedMemory(shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory destroyed\n");

    /* ---------------------------------------------------------------------- *
     *                        DESTROY SEMAPHORE (MUTEX)                       *
     * ---------------------------------------------------------------------- */
    if (destroySemaphore(mutex_shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Mutex destroyed\n");

    exit(EXIT_SUCCESS);
}

void handler_sigusr1_managePatient(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(val);

    printf("Patient recu, SIS = %d\n", siginfo_handler->si_value.sival_int);

    /* ---------------------------------------------------------------------- *
     *                              SEND MESSAGE                              *
     * ---------------------------------------------------------------------- */

//    if (sendMessage(&mq_traitement, "Hello Wordl", 1) == -1)
//        exit(EXIT_FAILURE);

//    GREENPRINTF("Message sent !\n");
}

