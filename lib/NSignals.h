#ifndef __NSIGNALS_H__
#define __NSIGNALS_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/**
 * @brief setSignalHandler : specify the action to be associated with a specific signal
 * @param signal : The argument signal specifies  the  signal
 * @param void(*) (int, siginfo_t *, void *) : Pointer to a signal-catching function.
 * @return : Upon successful completion, setSignalHandler() shall return 0; otherwise, −1 shall be returned
 */
int setSignalHandler(int signal, void (*p_fct)(int, siginfo_t*, int*));

/**
 * @brief setSignalHandler : specify the action to be associated with a specific signal
 * @param signal : The argument signal specifies  the  signal
 * @param void(*) (int, siginfo_t *, void *) : Pointer to a signal-catching function.
 * @param act : points to a structure specifying the action to be associated with the specified signal.
 * @param oact : the action previously associated with the signal is stored in the location pointed to by this argument.
 * @return : Upon successful completion, setSignalHandler() shall return 0; otherwise, −1 shall be returned
 */
int setAdvancedSignalHandler(int signal,
                             void (*p_fct)(int, siginfo_t*, int*),
                             struct sigaction* act,
                             struct sigaction* oact);

int sendQueuedSignal(int signal, pid_t pid_proccess, const union sigval value);


int blockSignal(int signal);
int blockOnlySignal(int signal);
int blockAllSignals(void);
int blockAllSignalsExceptedOne(int signal);
int blockSetOfSignals(const sigset_t* set, sigset_t* oldset);

int unblockSignal(int signal);
int unblockOnlySignal(int signal);
int unblockAllSignals(void);
int unblockAllSignalsExceptedOne(int signal);
int unblockSetOfSignals(const sigset_t* set, sigset_t* oldset);

/* -------------------------------------------------------------------------- *
 *                              PRIVATE FUNCTIONS                             *
 * -------------------------------------------------------------------------- */

int privateChangedBlockedSignals(
        int __how, const sigset_t* __set, sigset_t* __oldset);

#endif /* __NSIGNALS_H__ */
