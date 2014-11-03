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

int main(void)
{
    MessageQueue mq_traitement;
    SharedMemory shm;
    pid_t pid;

    /* ---------------------------------------------------------------------- *
     *                          CREATE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if (createSharedMemory(&shm, SHM_NAME, SHM_NB_RECORDS * sizeof(pid_t)) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory created\n");


    /* ---------------------------------------------------------------------- *
     *                         WRITE IN SHARED MEMORY                         *
     * ---------------------------------------------------------------------- */
    pid = getpid();
    printf("PID = %d\n", pid);
    writeSharedMemory(&shm, &pid, sizeof(pid_t), 0);

    GREENPRINTF("PID writen\n");

    /* ---------------------------------------------------------------------- *
     *                           OPEN MESSAGE QUEUE                           *
     * ---------------------------------------------------------------------- */
    if (openMessageQueue(&mq_traitement, MQ_NAME) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Message queue opened\n");

    /* ---------------------------------------------------------------------- *
     *                              SEND MESSAGE                              *
     * ---------------------------------------------------------------------- */

    if (sendMessage(&mq_traitement, "Hello Wordl", 1) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Message sent !\n");

    getchar();

    /* ---------------------------------------------------------------------- *
     *                           CLOSE MESSAGE QUEUE                          *
     * ---------------------------------------------------------------------- */

    if (closeMessageQueue(&mq_traitement) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Messsage queue closed\n");

    /* ---------------------------------------------------------------------- *
     *                          DESTROY SHARED MEMORY                         *
     * ---------------------------------------------------------------------- */
    if (destroySharedMemory(shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory destroyed\n");

    return EXIT_SUCCESS;
}

