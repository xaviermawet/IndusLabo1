/* -------------------------------------------------------------------------- *
 *                                   ACCUEIL                                  *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/common.h"
#include "../lib/NSemaphore.h"
#include "../lib/NSharedMemory.h"
#include "../lib/NSignals.h"
#include "../lib/NMessageQueue.h"
#include "../hospital.h"

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val);

int main(void)
{
    SharedMemory shm;
    pid_t pid_tri;

    // Free resources
    setSignalHandler(SIGINT, handler_sigint_exit);

    /* ---------------------------------------------------------------------- *
     *                          CREATE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if(openSharedMemory(&shm, SHM_NAME, SHM_NB_RECORDS * sizeof(pid_t)) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory opened\n");

    /* ---------------------------------------------------------------------- *
     *                          READ IN SHARED MEMORY                         *
     * ---------------------------------------------------------------------- */
    readSharedMemory(&shm, &pid_tri, sizeof(pid_t), 0);
    printf("PID tri : %d\n", pid_tri);

    /* ---------------------------------------------------------------------- *
     *                           CLOSE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if (closeSharedMemory(shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory closed\n");

    // Loop for receiving signals
    while(1)
        pause();

//    char buff[250];
//    MessageQueue mq;

//    printf("Création d'une message queue ...\n");

//    if (createMessageQueue(&mq, "/mqlinuxpedia", 10, 100) == -1)
//    {
//        printf("La message queue existe déjà --> ouverture...\n");
//        openMessageQueue(&mq, "/mqlinuxpedia");
//        printf("Message queue ouverte !\n");
//    }
//    else
//        printf("Message queue créée !\n");

//    printf("Envoi du message : Hello World\n");

//    if (sendMessage(&mq, "Hello World", 42) != -1)
//        printf("Message envoyé...\n");

//    sleep(2);

//    printf("Lecture du message ...\n");

//    receiveMessage(&mq, buff, 250, NULL);

//    printf("Message lu : %s\n", buff);

//    printf("Destruction...\n");

//    destroyMessageQueue(&mq);

//    printf("Message queue détruite !!!\n");



//    int i;
//    union sigval value;

//    printf("PID : %d\n", getpid());

//    srand(time(NULL));
//    for(i = 0; i < 10; ++i)
//        printf("Random Number : %d\n", getRandomNumber(0, 100));

//    printf("Armement du signal SIGINT\n");

//    setSignalHandler(SIGINT, handler_sigint);

//    sleep(10);

//    printf("Bloquage du signal SIGINT\n");
//    blockSignal(SIGINT);

//    sleep(5);

//    printf("Débloquage du signal SIGINT\n");
//    unblockSignal(SIGINT);

//    printf("Auto-envoi du signal SIGINT dans 2 sec ...\n");
//    sleep(2);

//    value.sival_int = 0;
//    sendQueuedSignal(SIGINT, getpid(), value);

//    // Loop for receiving signals
//    while(1)
//    {
//        pause();
//    }

    return EXIT_SUCCESS;
}

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    puts("Fermeture du programme...");
    exit(EXIT_SUCCESS);
}
