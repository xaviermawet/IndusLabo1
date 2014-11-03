#ifndef __HOSPITAL_H__
#define __HOSPITAL_H__

/* -------------------------------------------------------------------------- *
 *                                MESSAGE QUEUE                               *
 * -------------------------------------------------------------------------- */

#define MQ_NAME "/mqtraitement"
#define MQ_MAX_MESSAGE_LENGTH 100
#define MQ_MAX_NUM_OF_MESSAGES 10

/* -------------------------------------------------------------------------- *
 *                                SHARED MEMORY                               *
 * -------------------------------------------------------------------------- */

#define SHM_NAME "/shmtri"
#define SHM_NB_RECORDS 1

/* -------------------------------------------------------------------------- *
 *                                  SEMAPHORE                                 *
 * -------------------------------------------------------------------------- */

#define SEM_MUTEX_SHM_NAME "/tmp/sem_mutex_shm"

#endif /* __HOSPITAL_H__ */
