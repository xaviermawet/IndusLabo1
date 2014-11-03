/* -------------------------------------------------------------------------- *
 *                                   ACCUEIL                                  *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "../hospital.h"
#include "../lib/common.h"
#include "../lib/NSignals.h"
#include "../lib/NSemaphore.h"
#include "../lib/NSharedMemory.h"

// Handler
void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val);

// Functions
void DisplayMenu(void);
void generateSISNumbers(void);
void encodeSISNumbers(void);

pid_t pid_tri;

int main(void)
{
    Semaphore mutex_shm;
    SharedMemory shm;

    srand(time(NULL));

    // Free resources
    setSignalHandler(SIGINT, handler_sigint_exit);

    /* ---------------------------------------------------------------------- *
     *                         OPEN SEMAPHORE (MUTEX)                         *
     * ---------------------------------------------------------------------- */
    if (openSemaphore(&mutex_shm, SEM_MUTEX_SHM_NAME) == 1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Mutex opened\n");

    /* ---------------------------------------------------------------------- *
     *                           OPEN SHARED MEMORY                           *
     * ---------------------------------------------------------------------- */
    if(openSharedMemory(&shm, SHM_NAME, SHM_NB_RECORDS * sizeof(pid_t)) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory opened\n");

    /* ---------------------------------------------------------------------- *
     *                        READ PID IN SHARED MEMORY                       *
     * ---------------------------------------------------------------------- */
    readSharedMemory(&shm, &pid_tri, sizeof(pid_t), 0);

    waitSemaphore(mutex_shm);
    printf("PID process Tri : %d\n", pid_tri);
    signalSemaphore(mutex_shm);

    /* ---------------------------------------------------------------------- *
     *                           CLOSE SHARED MEMORY                          *
     * ---------------------------------------------------------------------- */
    if (closeSharedMemory(shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Shared memory closed\n");

    /* ---------------------------------------------------------------------- *
     *                         CLOSE SEMAPHORE (MUTEX)                        *
     * ---------------------------------------------------------------------- */
    if (closeSemaphore(mutex_shm) == -1)
        exit(EXIT_FAILURE);

    GREENPRINTF("Mutex closed\n");

    // Loop for receiving signals
    while(1)
        DisplayMenu();

    return EXIT_SUCCESS;
}

void handler_sigint_exit(int sig, siginfo_t* siginfo_handler, int* val)
{
    UNUSED(sig);
    UNUSED(siginfo_handler);
    UNUSED(val);

    GREENPRINTF("\nFree resources and close\n");
    exit(EXIT_SUCCESS);
}

void DisplayMenu(void)
{
    printf("=========================================\n");
    printf("|                ACCUEIL                |\n");
    printf("=========================================\n");
    printf("1. Generate random SIS numbers\n");
    printf("2. Encode SIS numbers\n");
    printf("Use CTRL + C to quit\n");

    printf("Your choice : ");

    switch (getNumber())
    {
        case 1:
            generateSISNumbers();
            break;
        case 2:
            encodeSISNumbers();
        default:
            break;
    }

    printf("Press ENTER to display menu");
    getchar();

    CLS
}

void generateSISNumbers(void)
{
    int i;
    int num_patients;
    int* SISNumbers = NULL;
    union sigval value;

    printf("How many patients : ");
    do
    {
        num_patients = getNumber();
    } while(num_patients < 0);

    SISNumbers = (int*)malloc(num_patients * sizeof(int));

    for(i = 0; i < num_patients; i++)
    {
        SISNumbers[i] = getRandomNumber(1, 10000);
        printf("Patient %d : SIS numbers %d\n", i + 1, SISNumbers[i]);
    }

    printf("Press ENTER to send all SIS numbers (patients) to the process Tri");
    getchar();

    for(i = 0; i < num_patients; i++)
    {
        value.sival_int = SISNumbers[i];
        sendQueuedSignal(SIGUSR1, pid_tri, value);
    }

    free(SISNumbers);

    GREENPRINTF("All SIS numbers sent\n");
}

void encodeSISNumbers(void)
{
    int i;
    int num_patients;
    int* SISNumbers = NULL;
    union sigval value;

    printf("How many patients : ");
    do
    {
        num_patients = getNumber();
    } while(num_patients < 0);

    SISNumbers = (int*)malloc(num_patients * sizeof(int));

    for(i = 0; i < num_patients; i++)
    {
        printf("Patient %d : SIS numbers : ", i + 1);
        SISNumbers[i] = getNumber();
    }

    printf("Press ENTER to send all SIS numbers (patients) to the process Tri");
    getchar();

    for(i = 0; i < num_patients; i++)
    {
        value.sival_int = SISNumbers[i];
        sendQueuedSignal(SIGUSR1, pid_tri, value);
    }

    free(SISNumbers);

    GREENPRINTF("All SIS numbers sent\n");
}
