/* -------------------------------------------------------------------------- *
 *                                 TRAITEMENT                                 *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/common.h"
#include "../lib/NSignals.h"
#include "../lib/NMessageQueue.h"
#include "../hospital.h"

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val);
void handler_sigusr1_readMessage(int sig, siginfo_t* siginfo_handler, int* val);

MessageQueue mq_traitement;
int c;

char message[MQ_MAX_MESSAGE_LENGTH];

int main(void)
{
    // Free resources
    setSignalHandler(SIGINT, handler_sigint_exit);

    /* ---------------------------------------------------------------------- *
     *                          CREATE MESSAGE QUEUE                          *
     * ---------------------------------------------------------------------- */
    if (createMessageQueue(&mq_traitement,
                       MQ_NAME,
                       MQ_MAX_NUM_OF_MESSAGES,
                       MQ_MAX_MESSAGE_LENGTH) == -1)
        REDPRINTF("Message queue creation failed\n");
    else
        GREENPRINTF("Message queue created\n");

    /* ---------------------------------------------------------------------- *
     *                        REGISTER FOR NOTIFICATION                       *
     * ---------------------------------------------------------------------- */

    setSignalHandler(SIGUSR1, handler_sigusr1_readMessage);

    if (getMessageQueueNotificationBySignal(&mq_traitement, SIGUSR1) == -1)
        REDPRINTF("Register for notification failed\n");
    else
        GREENPRINTF("Register for notification (SIGUSR1) OK\n");

    // Loop for receiving signals
    while(1)
    {
        pause();

        GREENPRINTF("Nouvel abonnement au signal ...\n");
        setSignalHandler(SIGUSR1, handler_sigusr1_readMessage);
    }
}

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    /* ---------------------------------------------------------------------- *
     *                          DESTROY MESSAGE QUEUE                         *
     * ---------------------------------------------------------------------- */
    if (closeAndDestroyMessageQueue(&mq_traitement) == -1)
        REDPRINTF("\nMessage queue destroy failed\n");
    else
        GREENPRINTF("\nMessage queue closed and destroyed\n");

    exit(EXIT_SUCCESS);
}

void handler_sigusr1_readMessage(int sig, siginfo_t* siginfo_handler, int* val)
{
    unsigned int priority;

    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    // Read current message
    if (receiveMessage(&mq_traitement, message, MQ_MAX_MESSAGE_LENGTH, &priority) == -1)
        REDPRINTF("Error when receiving message ...\n");

    printf("Message received with priority %d : %s\n", priority, message);


    // TODO : mettre la fille de message en non bloquant
    // TODO : la vider tant qu'il y a des messages
    // TODO : remettre la file en bloquant
    // TODO : faire une nouvelle demande de notification

    // TODO : vider le char[] message comme dans la lib pour vider le nom de je ne sais plus quoi ^^
}



