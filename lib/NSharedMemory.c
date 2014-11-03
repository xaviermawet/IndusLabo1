#include "NSharedMemory.h"

int createSharedMemory(SharedMemory* sharedMemory,
                       const char* name,
                       size_t size)
{
    sharedMemory->size = size;
    strcpy(sharedMemory->name, name);

    /* ---------------------------------------------------------------------- *
     * OUVERTURE D'UNE MEMOIRE PARTAGEE                                       *
     * IDENTIFICATEUR : sharedMemory                                          *
     * FLAGS          : (O_CREAT | O_RDWR)                                    *
     * MODE           : 0                                                     *
     * ---------------------------------------------------------------------- */

    if(privateOpenSharedMemory(sharedMemory, (O_CREAT | O_RDWR), 0) == -1)
        return -1;

    /* ---------------------------------------------------------------------- *
     * MODIFICATION DES PRIVILEGES D'ACCES D'UNE MEMOIRE PARTAGEE             *
     * IDENTIFICATEUR : sharedMemory->fildes                                  *
     * MODE           : S_IRRWXG | S_IRWXU | S_IRWXO                          *
     * ---------------------------------------------------------------------- */

    if (fchmod(sharedMemory->fildes, (mode_t)S_IRWXG | S_IRWXU | S_IRWXO) == -1)
    {
        perror("\n\rfchmod failed !!!\n");

        switch(errno)
        {
            case EBADF:
                perror("The fildes argument is not an open file descriptor.\n");
                break;
            case EPERM:
                perror("The effective user ID does not match the owner of the file and the process does not have appropriate privileges.\n");
                break;
            case EROFS:
                perror("The file referred to by fildes resides on a read-only file system.\n");
                break;
            case EINTR:
                perror("The fchmod() function was interrupted by a signal.\n");
                break;
            case EINVAL:
                perror("The value of the mode argument is invalid.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    /* ---------------------------------------------------------------------- *
     * MODIFICATION DE LA TAILLE D'UNE MEMOIRE PARTAGEE                       *
     * IDENTIFICATEUR : sharedMemory->fildes                                  *
     * TAILLE         : sharedMemory->size                                    *
     * ---------------------------------------------------------------------- */

    if (ftruncate(sharedMemory->fildes, sharedMemory->size) == -1)
    {
        perror("\n\rSet_Size_SHM (ftruncate) failed !!!\n");

        switch(errno)
        {
            case EINTR:
                perror("A signal was caught during execution.\n");
                break;
            case EINVAL:
                perror("The length argument was less than 0 or the length argument was greater than the maximum file size or the fildes argument is not a file descriptor open for writing.\n");
                break;
            case EFBIG:
                perror("The length argument was greater than the maximum file size.\n");
                break;
            case EIO:
                perror("An I/O error occurred while reading from or writing to a file system.\n");
                break;
            case EBADF:
                perror("The fildes argument is not a file descriptor open for writing.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    /* ---------------------------------------------------------------------- *
     * MAPPING D'UNE PAGE DANS LA MEMOIRE PARTAGEE                            *
     * IDENTIFICATEUR : sharedMemory                                          *
     * PROTECTION     : PROT_WRITE | PROT_READ                                *
     * FLAGS          : MAP_SHARED                                            *
     * ---------------------------------------------------------------------- */

    if (privateMmapSharedMemory(
                sharedMemory, (PROT_WRITE | PROT_READ), MAP_SHARED) == -1)
        return -1;

    return 0;
}

int openSharedMemory(SharedMemory* sharedMemory, const char* name, size_t size)
{
    sharedMemory->size = size;
    strcpy(sharedMemory->name, name);

    /* ---------------------------------------------------------------------- *
     * OUVERTURE D'UNE MEMOIRE PARTAGEE                                       *
     * IDENTIFICATEUR : sharedMemory                                          *
     * FLAGS          : (O_RDWR)                                              *
     * MODE           : 0                                                     *
     * ---------------------------------------------------------------------- */

    if (privateOpenSharedMemory(sharedMemory, (O_RDWR), 0) == -1)
        return -1;

    /* ---------------------------------------------------------------------- *
     * MAPPING D'UNE PAGE DANS LA MEMOIRE PARTAGEE                            *
     * IDENTIFICATEUR : sharedMemory                                          *
     * PROTECTION     : PROT_WRITE | PROT_READ                                *
     * FLAGS          : MAP_SHARED                                            *
     * ---------------------------------------------------------------------- */

    if (privateMmapSharedMemory(
                sharedMemory, (PROT_WRITE | PROT_READ), MAP_SHARED) == -1)
        return -1;

    return 0;
}

int writeSharedMemory(const SharedMemory* sharedMemory,
                      const void* data,
                      size_t sizeof_data,
                      unsigned int offset)
{
    memcpy(sharedMemory->ptr + (offset * sizeof_data), data, sizeof_data);

    return 0;
}

int readSharedMemory(const SharedMemory* sharedMemory,
                     void* data,
                     size_t sizeof_data,
                     unsigned int offset)
{
    memcpy(data, sharedMemory->ptr + (offset * sizeof_data), sizeof_data);

    return 0;
}

int closeSharedMemory(SharedMemory sharedMemory)
{
    /* ---------------------------------------------------------------------- *
     * DE-ALLOCATION D'UNE PAGE DE LA MEMOIRE PARTAGEE                        *
     * IDENTIFICATEUR : sharedMemory.ptr                                      *
     * LONGUEUR       : sharedMemory.size                                     *
     * ---------------------------------------------------------------------- */
    if (munmap(sharedMemory.ptr, sharedMemory.size) == -1)
    {
        perror("\n\rshm_munmap failed !!!\n");

        switch(errno)
        {
            case EINVAL:
                perror("Addresses in the range [addr,addr+len) are outside the valid range for the address space of a process or the len argument is 0 or the addr argument is not a multiple of the page size as returned by sysconf().\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }


    /* ---------------------------------------------------------------------- *
     * FERMETURE DE LA MEMOIRE PARTAGEE                                       *
     * IDENTIFICATEUR : sharedMemory.fildes                                   *
       ---------------------------------------------------------------------- */
    if (close(sharedMemory.fildes) == -1)
    {
        perror("\n\rshm_close failed !!!\n");

        switch(errno)
        {
            case EBADF:
                perror("The fildes argument is not a open file descriptor.\n");
                break;
            case EINTR:
                perror("The close() function was interrupted by a signal.\n");
                break;
            case EIO:
                perror("An I/O error occurred while reading from or writing to the file system.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;

    }

    return 0;
}

int closeAndDestroySharedMemory(SharedMemory sharedMemory)
{
    if (closeSharedMemory(sharedMemory) == -1)
        return -1;

    if (destroySharedMemory(sharedMemory) == -1)
        return -1;

    return 0;
}

int destroySharedMemory(SharedMemory sharedMemory)
{
    /* ---------------------------------------------------------------------- *
     * EFFACEMENT DE L'OBJET MEMOIRE PARTAGEE                                 *
     * NOM : sharedMemory.name.
       ---------------------------------------------------------------------- */
    if (shm_unlink(sharedMemory.name) == -1)
    {
        perror("\n\rshm_unlink failed !!!\n");

        switch(errno)
        {
            case EACCES:
                perror("Permission is denied to unlink the named shared memory object.\n");
                break;
            case ENAMETOOLONG:
                perror("The length of the name string exceeds PATH_MAX, or a pathname component is longer than NAME_MAX while _POSIX_NO_TRUNC is in effect.\n");
                break;
            case ENOENT:
                perror("The named shared memory object does not exist.\n");
                break;
            case ENOSYS:
                perror("The shm_unlink() function is not supported by the system.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

/* -------------------------------------------------------------------------- *
 *                             PRIVATE FONCTIONS                              *
 * -------------------------------------------------------------------------- */

int privateOpenSharedMemory(
        SharedMemory* sharedMemory, int __oflag, mode_t __mode)
{
    /* ---------------------------------------------------------------------- *
     * OUVERTURE D'UNE MEMOIRE PARTAGEE                                       *
     * NOM            : sharedMemory->name                                    *
     * IDENTIFICATEUR : sharedMemory->fildes                                  *
     * FLAGS          : __oflag                                               *
     * MODE           : __mode                                                *
     * ---------------------------------------------------------------------- */

    sharedMemory->fildes = shm_open(sharedMemory->name, __oflag, __mode);

    if(sharedMemory->fildes == -1)
    {
        perror("\n\rshm_open failed !!!\n");

        switch(errno)
        {
            case EACCES:
                perror("The shared memory object exists and the permissions specified by oflag are denied, or the shared memory object does not exist and permission to create the shared memory object is denied, or O_TRUNC is specified and write permission is denied.\n");
                break;
            case EEXIST:
                perror("Both O_CREAT and O_EXCL were specified to shm_open() and the shared memory object specified by name already exists.\n");
                break;
            case EINTR:
                perror("The shm_open() operation was interrupted by a signal.\n");
                break;
            case EINVAL:
                perror("The name argument to shm_open() was invalid.\n");
                break;
            case EMFILE:
                perror("The process already has the maximum number of files open.\n");
                break;
            case ENAMETOOLONG:
                perror("The length of name exceeds PATH_MAX.\n");
                break;
            case ENFILE:
                perror("The limit on the total number of files open on the system has been reached.\n");
                break;
            case ENOENT:
                perror("An attempt was made to shm_open() a name that did not exist, and O_CREAT was not specified.\n");
                break;
            case ENOSPC:
                perror("There is insufficient space for the creation of the new shared memory object.\n");
                break;
            case ENOSYS:
                perror("The function shm_open() is not supported by this implementation.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int privateMmapSharedMemory(
        SharedMemory* sharedMemory, int __protection, int __flags)
{
    /* ---------------------------------------------------------------------- *
     * MAPPING D'UNE PAGE DANS LA MEMOIRE PARTAGEE                            *
     * IDENTIFICATEUR : sharedMemory->fildes                                  *
     * LONGUEUR       : sharedMemory->size                                    *
     * PROTECTION     : __protection                                          *
     * FLAGS          : __flags                                               *
     * ---------------------------------------------------------------------- */

    sharedMemory->ptr = mmap(0, sharedMemory->size, __protection,
                             __flags, sharedMemory->fildes, 0);

    if (*sharedMemory->ptr == -1)
    {
        perror("\n\rmmap failed !!!\n");

        switch(errno)
        {
            case EACCES:
                perror("The fildes argument is not open for read, regardless of the protection specified, or fildes is not open for write and PROT_WRITE was specified for a MAP_SHARED type mapping.\n");
                break;
            case EAGAIN:
                perror("The mapping could not be locked in memory, if required by mlockall(), due to a lack of resources.\n");
                break;
            case EBADF:
                perror("The fildes argument is not a valid open file descriptor.\n");
                break;
            case EINVAL:
                perror("The value of len is zero or the value of flags is invalid (neither MAP_PRIVATE nor MAP_SHARED is set).\n");
                break;
            case EMFILE:
                perror("The number of mapped regions would exceed an implementation-defined limit (per process or per system).\n");
                break;
            case ENODEV:
                perror("The fildes argument refers to a file whose type is not supported by mmap().\n");
                break;
            case ENOMEM:
                perror("MAP_FIXED was specified, and the range [addr,addr+len) exceeds that allowed for the address space of a process; or, if MAP_FIXED was not specified and  there  is  insufficient room in the address space to effect the mapping or the mapping could not be locked in memory, if required by mlockall(), because it would require more space than the system is able to supply or not enough unallocated memory resources remain in the typed memory object designated by fildes to allocate len bytes.\n");
                break;
            case ENOTSUP:
                perror("MAP_FIXED or MAP_PRIVATE was specified in the flags argument and the implementation does not support this functionality. The implementation does not support the combination of accesses requested in the prot argument.\n");
                break;
            case ENXIO:
                perror("Addresses in the range [off,off+len) are invalid for the object specified by fildes or MAP_FIXED was specified in flags and the combination of addr, len, and off is invalid for the object specified by fildes or the fildes argument refers to a typed memory object that is not accessible from the calling process.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}
