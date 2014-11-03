#include "NSemaphore.h"

int createSemaphore(Semaphore* semaphore, const char* name, unsigned int value)
{
    semaphore->oflag = (O_CREAT | O_RDWR);
    semaphore->mode = 0644;
    semaphore->value = value;
    strcpy(semaphore->name, name);

    /* ---------------------------------------------------------------------- *
     * CREATION D'UN SEMAPHORE NOMME                                          *
     * IDENTIFICATEUR  : semaphore->p_sem                                     *
     * NOM             : semaphore->name[] = name                             *
     * FLAGS           : semaphore->oflags = (O_CREAT | O_RDWR)               *
     * MODE            : semaphore->mode = 644                                *
     * VALEUR INITIALE : semaphore->value = value                             *
     * ---------------------------------------------------------------------- */

    semaphore->p_sem = sem_open(semaphore->name,
                                semaphore->oflag,
                                semaphore->mode,
                                semaphore->value);

    if (semaphore->p_sem == (void *) -1)
    {
        perror("\n\rsem_open flag O_CREAT | O_RDWR and mode 644 failed !!!\n");

        switch(errno)
        {
            case EACCES :
                perror("The semaphore exists, but you do not have permission to open it\n");
                break;
            case EEXIST :
                perror("A semaphore with this name already exists\n");
                break;
            case EINTR :
                perror("Operation interrupted by a signal\n");
                break;
            case EINVAL :
                perror("The semaphore name or value is not valid\n");
                break;
            case EMFILE :
                perror("The process already has the maximum number of files and open\n");
                break;
            case ENAMETOOLONG :
                perror("The name was too long\n");
                break;
            case ENFILE :
                perror("The system limit on the total number of open files has been reached\n");
                break;
            case ENOMEM :
                perror("Insufficient memory to create the semaphore\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        destroySemaphore(*semaphore);

        return -1;
    }

    return 0;
}

int openSemaphore(Semaphore* semaphore, const char* name)
{
    semaphore->oflag = O_RDWR;
    semaphore->mode  = 0644;
    semaphore->value = 0;
    strcpy(semaphore->name, name);

    /* ---------------------------------------------------------------------- *
     * CREATION D'UN SEMAPHORE NOMME                                          *
     * IDENTIFICATEUR  : semaphore->p_sem                                     *
     * NOM             : semaphore->name[] = name                             *
     * FLAGS           : semaphore->oflags = O_RDWR                           *
     * MODE            : semaphore->mode   = 644                              *
     * VALEUR INITIALE : semaphore->value  = 0                                *
     * ---------------------------------------------------------------------- */
    semaphore->p_sem = sem_open(semaphore->name,
                                semaphore->oflag,
                                semaphore->mode,
                                semaphore->value);

    if (semaphore->p_sem == (void *) -1)
    {
        perror("\n\rsem_open flag 0_RDWR, mode 644, value 0 failed !!!\n");

        switch(errno)
        {
            case EACCES :
                perror("The semaphore exists, but you do not have permission to open it\n");
                break;
            case EINTR :
                perror("Operation interrupted by a signal\n");
                break;
            case EINVAL :
                perror("The semaphore name or value is not valid\n");
                break;
            case EMFILE :
                perror("The process already has the maximum number of files and open\n");
                break;
            case ENAMETOOLONG :
                perror("The name was too long\n");
                break;
            case ENFILE :
                perror("The system limit on the total number of open files has been reached\n");
                break;
            case ENOENT :
                perror("The Semaphore does not exists\n");
                break;
            case ENOMEM :
                perror("Insufficient memory to create the semaphore\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int closeSemaphore(Semaphore semaphore)
{
    int ret;

    /* ---------------------------------------------------------------------- *
     * FERMETURE D'UN SEMAPHORE NOMME                                         *
     * IDENTIFICATEUR : semaphore.p_sem                                       *
     * ---------------------------------------------------------------------- */

    if ((ret = sem_close(semaphore.p_sem)) == -1)
    {
        perror("\n\rsem_close failed !!!\n");

        switch(errno)
        {
            case EINVAL:
                perror("Not a valid semaphore descriptor\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }
    }

    return ret;
}

int closeAndDestroySemaphore(Semaphore semaphore)
{
    if (closeSemaphore(semaphore) == -1)
        return -1;

    if (destroySemaphore(semaphore) == -1)
        return -1;

    return 0;
}

int destroySemaphore(Semaphore semaphore)
{
    /* ---------------------------------------------------------------------- *
     * DESTRUCTION D'UN SEMAPHORE NOMME                                       *
     * NOM : semaphore.name[]                                                 *
     * ---------------------------------------------------------------------- */

    if (sem_unlink(semaphore.name) == -1)
    {
        perror("\n\rsem_unlink failed !!!\n");

        switch(errno)
        {
            case EACCES :
                perror("You do not have permission to unlink this semaphore\n");
                break;
            case ENAMETOOLONG :
                perror("The name was too long\n");
                break;
            case ENOENT :
                perror("There is no semaphore with the given name\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int waitSemaphore(Semaphore semaphore)
{
    int ret;

    /* ---------------------------------------------------------------------- *
     * ATTENDRE UN SEMAPHORE                                                  *
     * IDENTIFICATEUR : sem.p_sem                                             *
     * ---------------------------------------------------------------------- */

    if ((ret = sem_wait(semaphore.p_sem)) == -1)
    {
        perror("\n\rsem_wait failed !!!\n");

        switch(errno)
        {
            case EINTR :
                perror("The call was interrupted by a signal handler\n");
                break;
            case EINVAL :
                perror("The semaphore is not a valid semaphore\n");
                break;
            case EAGAIN :
                perror("The operation could not be performed without blocking\n");
                break;
            case ETIMEDOUT:
                perror("The call timed out before the semaphore could be locked\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }
    }

    return ret;
}

int signalSemaphore(Semaphore semaphore)
{
    int ret;

    /* ---------------------------------------------------------------------- *
     * VALIDER UN SEMAPHORE                                                   *
     * IDENTIFICATEUR : sem.p_sem                                             *
     * ---------------------------------------------------------------------- */

    if ((ret = sem_post(semaphore.p_sem)) == -1)
    {
        perror("\n\rsem_post failed !!!\n");

        switch(errno)
        {
            case EINVAL:
                perror("Not a valid semaphore descriptor\n");
                break;
            case EOVERFLOW:
                perror("The maximum allowable value for a semaphore would be exceeded\n");
                break;
            default :
                perror("Unknown error\n");
                break;
        }
    }

    return ret;
}


int getSemaphoreValue(Semaphore const* semaphore)
{
    int sem_val = -1;

    /* ---------------------------------------------------------------------- *
     * RECUPERER LA VALEUR D'UN SEMAPHORE                                     *
     * IDENTIFICATEUR : sem.p_sem                                             *
     * VALEUR         : &sem_val                                              *
     * ---------------------------------------------------------------------- */

    if (sem_getvalue(semaphore->p_sem, &sem_val) == -1)
    {
        perror("\n\rsem_getvalue failed !!!\n");

        switch (errno)
        {
            case EINVAL:
                perror("Not a valid semaphore descriptor\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return sem_val;
}
