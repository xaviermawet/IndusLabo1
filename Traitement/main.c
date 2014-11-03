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
        pause();
}

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    /* ---------------------------------------------------------------------- *
     *                          DESTROY MESSAGE QUEUE                         *
     * ---------------------------------------------------------------------- */
    if (destroyMessageQueue(&mq_traitement) == -1)
        REDPRINTF("Message queue destroy failed\n");
    else
        GREENPRINTF("Message queue destroyed\n");

    exit(EXIT_SUCCESS);
}

void handler_sigusr1_readMessage(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    GREENPRINTF("Lire message ...\n");
    FREE_BUFF
}



