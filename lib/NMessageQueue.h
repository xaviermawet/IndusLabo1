#ifndef __NMESSAGEQUEUE_H__
#define __NMESSAGEQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <mqueue.h>
#include <stdarg.h>

#define MQ_NAME_LENGTH 80          // May contain the whole path to the file
#define MQ_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) // 0644

typedef struct MessageQueue_t
{
    mqd_t mq_des;                    // Message queue file descriptor
    char name[MQ_NAME_LENGTH];
    struct mq_attr attributes;
} MessageQueue;

int createMessageQueue(MessageQueue* messageQueue,
                       const char* name,
                       long max_msg_count,
                       long max_msg_size);

int openMessageQueue(MessageQueue* messageQueue,
                     const char* name);

int closeMessageQueue(MessageQueue* messageQueue);

int destroyMessageQueue(MessageQueue* messageQueue);

int closeAndDestroyMessageQueue(MessageQueue* messageQueue);

int sendMessage(const MessageQueue* messageQueue,
                const char* message,
                unsigned int priority);

int receiveMessage(const MessageQueue* messageQueue,
                   char* msg,
                   size_t msg_max_len,
                   unsigned int* msg_priority);

int getMessageQueueNotificationBySignal(const MessageQueue* messageQueue,
                                        int signal);

int stopMessageQueueNotificationBySignal(const MessageQueue* messageQueue);

/* -------------------------------------------------------------------------- *
 *                              PRIVATE FUNCTIONS                             *
 * -------------------------------------------------------------------------- */

/**
 * @brief privateOpenMessageQueue : open a message queue
 *        Optional parameter must be mode_t __mode if O_CREAT is set in __oflag
 * @param messageQueue : contains all required information for the message queue
 * @param __oflag : specifies flags that control the operation of the call.
 * @return returns 0 on success, or -1 on error
 */
int privateOpenMessageQueue(MessageQueue* messageQueue,
                            int __oflag,
                            ...);

int privateMessageQueueNotificationBySignal(const MessageQueue* messageQueue,
                                            const struct sigevent* sevp);

#endif /* __NMESSAGEQUEUE_H__ */
