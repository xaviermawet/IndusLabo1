#ifndef __NSEMAPHORE_H__
#define __NSEMAPHORE_H__

#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h> /* Pour les constantes "mode" */
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* Pour les constantes O_* */
#include <errno.h>

#define SEM_NAME_LENGTH 40

typedef struct Semaphore_t
{
    sem_t*       p_sem;
    int          oflag;
    mode_t       mode;
    char         name[SEM_NAME_LENGTH];
    unsigned int value;
} Semaphore;

/**
 * @brief createSemaphore : create and open a new named semaphore
 * @param semaphore : struct than will contain all parameters for the semaphore
 * @param name : name for the new semaphore
 * @param value : valeur for the new semaphore
 * @return : 0 if the semaphore is created & opened without error, otherwise -1
 */
int createSemaphore(Semaphore* semaphore, const char* name, unsigned int value);

/**
 * @brief openSemaphore : opens an existing named semaphore identified by name
 * @param semaphore : struct than will contain all parameters for the semaphore
 * @param name : name for the semaphore
 * @return : 0 if the semaphore is opened without error, otherwise -1
 */
int openSemaphore(Semaphore* semaphore, const char* name);

/**
 * @brief closeSemaphore : close a named semaphore
 * @param semaphore : struct that contains all parameters for the semaphore
 * @return : 0 if the semaphore is closed without error, otherwise -1
 */
int closeSemaphore(Semaphore semaphore);

/**
 * @brief destroySemaphore : destroy a named semaphore
 * @param Semaphore : struct that contains all parameters for the semaphore
 * @return : 0 if the semaphore is destroyed without error, otherwise -1
 */
int destroySemaphore(Semaphore semaphore);

/**
 * @brief destroySemaphore : close and destroy a named semaphore
 * @param Semaphore : struct that contains all parameters for the semaphore
 * @return : 0 if the semaphore is destroyed without error, otherwise -1
 */
int closeAndDestroySemaphore(Semaphore semaphore);

/**
 * @brief waitSemaphore : decrements (locks) the semaphore
 * @param semaphore : struct that contains all parameters for the semaphore
 * @return : 0 if the semaphore is decreased without error, otherwise -1
 */
int waitSemaphore(Semaphore semaphore);

/**
 * @brief signalSemaphore : increase the value of the semaphore
 * @param semaphore : struct that contains all parameters for the semaphore
 * @return 0 if the semaphore is increased without error, otherwise -1
 */
int signalSemaphore(Semaphore semaphore);

/**
 * @brief getSemaphoreValue : get the value of a named semaphore
 * @param semaphore
 * @return : returns the semaphore value on success; on error, -1 is returned
 *           and errno is set to indicate the error
 */
int getSemaphoreValue(Semaphore const* semaphore);


#endif /* __NSEMAPHORE_H__ */
