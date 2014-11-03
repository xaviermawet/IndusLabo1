#include "NMessageQueue.h"

int createMessageQueue(MessageQueue* messageQueue,
                       const char *name,
                       long max_msg_count,
                       long max_msg_size)
{
    strcpy(messageQueue->name, name);

    messageQueue->attributes.mq_maxmsg  = max_msg_count;  /* Max. # of messages on queue */
    messageQueue->attributes.mq_msgsize = max_msg_size;   /* Max. message size (bytes) */
    messageQueue->attributes.mq_flags   = 0;              /* Flags: 0 or O_NONBLOCK */
    messageQueue->attributes.mq_curmsgs = 0;              /* # of messages currently in queue */

    return privateOpenMessageQueue(messageQueue,
                                   (O_CREAT | O_EXCL | O_RDWR ),
                                   (mode_t) MQ_MODE);
}

int openMessageQueue(MessageQueue* messageQueue,
                     const char* name)
{
    strcpy(messageQueue->name, name);

    return privateOpenMessageQueue(messageQueue, (O_RDWR));
}

int closeMessageQueue(const MessageQueue* messageQueue)
{
    if (mq_close(messageQueue->mq_des) == -1)
    {
        perror("\n\rmq_close failed !!\n");

        switch(errno)
        {
            case EBADF:
                perror("The descriptor specified in mqdes is invalid.\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int destroyMessageQueue(const MessageQueue* messageQueue)
{
    if (closeMessageQueue(messageQueue) == -1)
        return -1;

    if (mq_unlink(messageQueue->name) == -1)
    {
        perror("\n\rmq_unlink failed !!!\n");

        switch(errno)
        {
            case EACCES:
                perror("The caller does not have permission to unlink this message queue.\n");
                break;
            case ENAMETOOLONG:
                perror("Name was too long.\n");
                break;
            case ENOENT:
                perror("There is no message queue with the given name.\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int sendMessage(const MessageQueue* messageQueue,
                const char* message,
                unsigned int priority)
{
    if (mq_send(messageQueue->mq_des, message, strlen(message) + 1, priority) == -1)
    {
        perror("\n\rmq_send failed !!!\n");

        switch(errno)
        {
            case EAGAIN:
                perror("The queue was full, and the O_NONBLOCK flag was set for the message queue description referred to by mqdes.\n");
                break;
            case EBADF:
                perror("The descriptor specified in mqdes was invalid.\n");
                break;
            case EINTR:
                perror("The call was interrupted by a signal handler.\n");
                break;
            case EINVAL:
                perror("The call would have blocked, and abs_timeout was invalid, either because tv_sec was less than zero, or because tv_nsec was less than zero or greater than 1000 million.\n");
                break;
            case EMSGSIZE:
                perror("msg_len was greater than the mq_msgsize attribute of the message queue.\n");
                break;
            case ETIMEDOUT:
                perror("The call timed out before a message could be transferred.\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int receiveMessage(const MessageQueue* messageQueue,
                   char* msg,
                   size_t msg_max_len,
                   unsigned int* msg_priority)
{
    ssize_t msg_len;

    msg_len = mq_receive(messageQueue->mq_des, msg, msg_max_len, msg_priority);

    if (msg_len < 0)
    {
        perror("\n\rmq_receive failed !!!\n");

        switch(errno)
        {
            case EAGAIN:
                perror("The queue was empty, and the O_NONBLOCK flag was set for the message queue description referred to by mqdes.\n");
                break;
            case EBADF:
                perror("The descriptor specified in mqdes was invalid.\n");
                break;
            case EINTR:
                perror("The call was interrupted by a signal handler.\n");
                break;
            case EINVAL:
                perror("The call would have blocked, and abs_timeout was invalid, either because tv_sec was less than zero, or because tv_nsec was less than zero or greater than 1000 million.\n");
                break;
            case EMSGSIZE:
                perror("msg_len was less than the mq_msgsize attribute of the message queue.\n");
                break;
            case ETIMEDOUT:
                perror("The call timed out before a message could be transferred.\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    msg[msg_max_len -1] = '\0';

    return (int)msg_len;
}

int getMessageQueueNotificationBySignal(const MessageQueue* messageQueue,
                                        int signal)
{
    struct sigevent notification;
    notification.sigev_notify = SIGEV_SIGNAL;
    notification.sigev_signo  = signal;

    return privateMessageQueueNotificationBySignal(messageQueue, &notification);
}

int stopMessageQueueNotificationBySignal(const MessageQueue *messageQueue)
{
    return privateMessageQueueNotificationBySignal(messageQueue, NULL);
}

/* -------------------------------------------------------------------------- *
 *                              PRIVATE FUNCTIONS                             *
 * -------------------------------------------------------------------------- */

int privateOpenMessageQueue(MessageQueue* messageQueue,
                            int __oflag,
                            ...)
{
    /* Check if O_CREATE is set in __oflag */

    if ((__oflag & O_CREAT) == O_CREAT) // O_CREAT is set
    {
        // Get __mode parameter
        mode_t __mode;
        va_list ap;

        va_start(ap, __oflag);
        __mode = va_arg(ap, mode_t);
        va_end(ap);

        messageQueue->mq_des = mq_open(messageQueue->name,
                                       __oflag,
                                       __mode,
                                       &messageQueue->attributes);
    }
    else // O_CREAT is NOT set
        messageQueue->mq_des = mq_open(messageQueue->name,
                                       __oflag);

    if (messageQueue->mq_des == (mqd_t) -1)
    {
            perror("\n\rmq_open failed !!!\n");

            switch(errno)
            {
                case EACCES:
                    perror("The queue exists, but the caller does not have permission to open it in the specified mode or name contained more than one slash.\n");
                    break;
                case EEXIST:
                    perror("Both O_CREAT and O_EXCL were specified in oflag, but a queue with this name already exists.\n");
                    break;
                case EINVAL:
                    perror("O_CREAT was specified in oflag, and attr was not NULL, but attr->mq_maxmsg or attr->mq_msqsize was invalid.  Both of these fields must be greater than zero.\n");
                    break;
                case EMFILE:
                    perror("The process already has the maximum number of files and message queues open.\n");
                    break;
                case ENAMETOOLONG:
                    perror("Name was too long.\n");
                    break;
                case ENFILE:
                    perror("The system limit on the total number of open files and message queues has been reached.\n");
                    break;
                case ENOENT:
                    perror("The O_CREAT flag was not specified in oflag, and no queue with this name exists or name was just '/' followed by no other characters.\n");
                    break;
                case ENOMEM:
                    perror("Insufficient memory.\n");
                    break;
                case ENOSPC:
                    perror("Insufficient space for the creation of a new message queue.  This probably occurred because the queues_max limit was encountered.\n");
                    break;
                default:
                    perror("Unknown error\n");
                    break;
            }

            return -1;
        }

    return 0;
}

int privateMessageQueueNotificationBySignal(const MessageQueue* messageQueue,
                                            const struct sigevent* sevp)
{
    if (mq_notify(messageQueue->mq_des, sevp) == -1)
    {
        perror("\n\rmq_notify failed !!!\n");

        switch(errno)
        {
            case EBADF:
                perror("The descriptor specified in mqdes is invalid.\n");
                break;
            case EBUSY:
                perror("Another process has already registered to receive notification for this message queue.\n");
                break;
            case EINVAL:
                perror("sevp->sigev_notify is not one of the permitted values; or sevp->sigev_notify is SIGEV_SIGNAL and sevp->sigev_signo is not a valid signal number.\n");
                break;
            case ENOMEM:
                perror("Insufficient memory.\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}
