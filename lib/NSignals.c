#include "NSignals.h"

int setSignalHandler(int signal, void (*p_fct)(int, siginfo_t *, int *))
{
    struct sigaction act;

    return setAdvancedSignalHandler(signal, p_fct, &act, NULL);
}

int setAdvancedSignalHandler(int signal,
                             void (*p_fct)(int, siginfo_t *, int *),
                             struct sigaction* act,
                             struct sigaction* oact)
{
    /* POSIX requires that we use sa_sigaction if sa_flags includes SA_SIGINFO,
     * and that we use sa_handler otherwise. But we still use sa_sigaction
     * when SA_SIGINFO is not defined. Practice says it will work, but theory
     * says we can't rely on it to work.
     */

    act->sa_sigaction = (void(*)(int, siginfo_t*, void*)) p_fct;

    sigemptyset(&act->sa_mask);

    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field,
     * not sa_handler.
     */
    act->sa_flags = SA_SIGINFO;

    if(sigaction(signal, act, oact) < 0)
    {
        perror("\n\rError sigaction !!!\n");

        switch(errno)
        {
            case EINVAL:
                perror("The sig argument is not a valid signal number or an attempt is made to catch a signal that cannot be caught or ignore a signal that cannot be ignored.\n");
                break;
            case ENOTSUP:
                perror("The SA_SIGINFO bit flag is set in the sa_flags field of the sigaction structure.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int sendQueuedSignal(int signal, pid_t pid_proccess, const union sigval value)
{
    if(sigqueue(pid_proccess, signal, value) == -1)
    {
        perror("\n\rError sigqueue !!!\n");

        switch(errno)
        {
            case EAGAIN:
                perror("The limit of signals which may be queued has been reached.\n");
                break;
            case EINVAL:
                perror("invalid signal\n.");
                break;
            case EPERM:
                perror("The process does not have permission to send the signal to the receiving process.\n");
                break;
            case ESRCH:
                perror("No process has a PID matching pid_proccess.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}

int blockSignal(int signal)
{
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, signal);

    return privateChangedBlockedSignals(SIG_BLOCK, &signal_set, NULL);
}

int blockOnlySignal(int signal)
{
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, signal);

    return privateChangedBlockedSignals(SIG_SETMASK, &signal_set, NULL);
}

int blockAllSignals(void)
{
    sigset_t signal_set;
    sigfillset(&signal_set);

    return privateChangedBlockedSignals(SIG_SETMASK, &signal_set, NULL);
}

int blockAllSignalsExceptedOne(int signal)
{
    sigset_t signal_set;
    sigfillset(&signal_set);
    sigdelset(&signal_set, signal);

    return privateChangedBlockedSignals(SIG_SETMASK, &signal_set, NULL);
}

int blockSetOfSignals(const sigset_t* set, sigset_t* oldset)
{
    return privateChangedBlockedSignals(SIG_BLOCK, set, oldset);
}

int unblockSignal(int signal)
{
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, signal);

    return privateChangedBlockedSignals(SIG_UNBLOCK, &signal_set, NULL);
}

int unblockOnlySignal(int signal)
{
    return blockAllSignalsExceptedOne(signal);
}

int unblockAllSignals(void)
{
    sigset_t signal_set;
    sigfillset(&signal_set);

    return privateChangedBlockedSignals(SIG_UNBLOCK, &signal_set, NULL);
}

int unblockAllSignalsExceptedOne(int signal)
{
    return blockOnlySignal(signal);
}

int unblockSetOfSignals(const sigset_t* set, sigset_t* oldset)
{
    return privateChangedBlockedSignals(SIG_UNBLOCK, set, oldset);
}

int privateChangedBlockedSignals(
        int __how, const sigset_t* __set, sigset_t* __oldset)
{
    if (sigprocmask(__how, __set, __oldset) == -1)
    {
        perror("\n\rError sigprocmask !!!\n");

        switch(errno)
        {
            case EFAULT:
                perror("the set or oldset argument points outside the process's allocated address space.\n");
                break;
            case EINVAL:
                perror("The value specified in how was invalid.\n");
                break;
            default:
                perror("Unknown error\n");
                break;
        }

        return -1;
    }

    return 0;
}
