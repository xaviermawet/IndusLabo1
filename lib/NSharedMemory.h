#ifndef __NSHAREDMEMORY_H__
#define __NSHAREDMEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h> /* Pour les constantes "mode" */
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* Pour les constantes O_* */
#include <errno.h>

#define SHM_NAME_LENGTH 80 // May contain the whole path to the file

typedef struct SharedMemory_t
{
    int     fildes;
    caddr_t ptr;
    char    name[SHM_NAME_LENGTH];
    size_t  size;
} SharedMemory;

int createSharedMemory(
        SharedMemory* sharedMemory, const char* name, size_t size);

int openSharedMemory(
        SharedMemory* sharedMemory, const char* name, size_t size);

int writeSharedMemory(
        const SharedMemory* sharedMemory,
        const void* data,
        size_t sizeof_data,
        unsigned int offset);

int readSharedMemory(
        const SharedMemory* sharedMemory,
        void* data,
        size_t sizeof_data,
        unsigned int offset);

int closeSharedMemory(SharedMemory sharedMemory);

int destroySharedMemory(SharedMemory sharedMemory);

/* -------------------------------------------------------------------------- *
 *                             PRIVATE FONCTIONS                              *
 * -------------------------------------------------------------------------- */

int privateOpenSharedMemory(
        SharedMemory* sharedMemory, int __oflag, mode_t __mode);

int privateMmapSharedMemory(
        SharedMemory* sharedMemory, int __protection, int __flags);

#endif /* __NSHAREDMEMORY_H__ */
