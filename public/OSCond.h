#ifndef __OSCOND_H_
#define __OSCOND_H_

#ifndef __Win32__
    #if __PTHREADS_MUTEXES__
        #include <pthread.h>
    #else
//        #include "mycondition.h"
    #endif
#endif

#include "stdint.h"
#include "OSMutex.h"

namespace PublicTool {

    class OSMutex;
    class OSCond
    {
        public:

            OSCond();
            ~OSCond();

            inline void     Signal();
            inline void     Wait(OSMutex* inMutex, int32_t inTimeoutInMilSecs = 0);
            inline void     Broadcast();

        private:

    #ifdef __Win32__
            HANDLE              fCondition;
            uint32_t              fWaitCount;
    #elif __PTHREADS_MUTEXES__
            pthread_cond_t      fCondition;
            void                TimedWait(OSMutex* inMutex, int32_t inTimeoutInMilSecs);
    #else
//            mycondition_t       fCondition;
    #endif
    };


    inline void OSCond::Wait(OSMutex* inMutex, int32_t inTimeoutInMilSecs)
    {
    #ifdef __Win32__
        DWORD theTimeout = INFINITE;
        if (inTimeoutInMilSecs > 0)
            theTimeout = inTimeoutInMilSecs;
        inMutex->Unlock();
        fWaitCount++;
        DWORD theErr = ::WaitForSingleObject(fCondition, theTimeout);
        fWaitCount--;
        Assert((theErr == WAIT_OBJECT_0) || (theErr == WAIT_TIMEOUT));
        inMutex->Lock();
    #elif __PTHREADS_MUTEXES__
        this->TimedWait(inMutex, inTimeoutInMilSecs);
    #else
//        Assert(fCondition != NULL);
//        mycondition_wait(fCondition, inMutex->fMutex, inTimeoutInMilSecs);
    #endif
    }

    inline void OSCond::Signal()
    {
    #ifdef __Win32__
        BOOL theErr = ::SetEvent(fCondition);
        Assert(theErr == TRUE);
    #elif __PTHREADS_MUTEXES__
        pthread_cond_signal(&fCondition);
    #else
//        Assert(fCondition != NULL);
//        mycondition_signal(fCondition);
    #endif
    }

    inline void OSCond::Broadcast()
    {
    #ifdef __Win32__
        //
        // There doesn't seem like any more elegant way to
        // implement Broadcast using events in Win32.
        // This will work, it may generate spurious wakeups,
        // but condition variables are allowed to generate
        // spurious wakeups
        uint32_t waitCount = fWaitCount;
        for (uint32_t x = 0; x < waitCount; x++)
        {
            BOOL theErr = ::SetEvent(fCondition);
            Assert(theErr == TRUE);
        }
    #elif __PTHREADS_MUTEXES__
        pthread_cond_broadcast(&fCondition);
    #else
//        Assert(fCondition != NULL);
//        mycondition_broadcast(fCondition);
    #endif
    }
}
#endif // OSCond_H
