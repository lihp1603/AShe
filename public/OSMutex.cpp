#include "OSMutex.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "loger/simpleLoger.h"

namespace PublicTool {


    // Private globals
    #if __PTHREADS_MUTEXES__
        static pthread_mutexattr_t  *sMutexAttr=NULL;
        static void MutexAttrInit();

        #if __solaris__
                static pthread_once_t sMutexAttrInit = {PTHREAD_ONCE_INIT};
        #else
                static pthread_once_t sMutexAttrInit = PTHREAD_ONCE_INIT;
        #endif

    #endif

    OSMutex::OSMutex()
    {
    #ifdef __Win32__
        ::InitializeCriticalSection(&fMutex);
        fHolder = 0;
        fHolderCount = 0;
    #elif __PTHREADS_MUTEXES__
        (void)pthread_once(&sMutexAttrInit, MutexAttrInit);
        (void)pthread_mutex_init(&fMutex, sMutexAttr);

        fHolder = 0;
        fHolderCount = 0;
    #else
        fMutex = mymutex_alloc();
    #endif
    }

    #if __PTHREADS_MUTEXES__
    void MutexAttrInit()
    {
        sMutexAttr = (pthread_mutexattr_t*)malloc(sizeof(pthread_mutexattr_t));
        ::memset(sMutexAttr, 0, sizeof(pthread_mutexattr_t));
        pthread_mutexattr_init(sMutexAttr);
    }
    #endif

    OSMutex::~OSMutex()
    {
    #ifdef __Win32__
        ::DeleteCriticalSection(&fMutex);
    #elif __PTHREADS_MUTEXES__
        pthread_mutex_destroy(&fMutex);
    #else
        mymutex_free(fMutex);
    #endif
    }


    #if __PTHREADS_MUTEXES__ || __Win32__
    void        OSMutex::RecursiveLock()
    {
        // We already have this mutex. Just refcount and return
        if (CThread::GetCurrentThreadID() == fHolder)
        {
            fHolderCount++;
            return;
        }
    #ifdef __Win32__
        ::EnterCriticalSection(&fMutex);
    #else
        (void)pthread_mutex_lock(&fMutex);
    #endif
        if(fHolder == 0){
            fHolder = CThread::GetCurrentThreadID();
            fHolderCount++;
            assert(fHolderCount == 1);
        }else{
             LogTraceI("maybe Repeat locked");
        }
        //这种写法会出现崩溃的情况
//        assert(fHolder == 0);
//        fHolder = CThread::GetCurrentThreadID();
//        fHolderCount++;
//        assert(fHolderCount == 1);
    }

    void        OSMutex::RecursiveUnlock()
    {
        if (CThread::GetCurrentThreadID() != fHolder){
            LogTraceI("maybe happed a bug");
            return;
        }

        assert(fHolderCount > 0);
        fHolderCount--;
//        LogTraceI("fHolder -- =%d",fHolder);
        if (fHolderCount == 0)
        {
            fHolder = 0;
    #ifdef __Win32__
            ::LeaveCriticalSection(&fMutex);
    #else
            pthread_mutex_unlock(&fMutex);
    #endif
        }
    }

    bool      OSMutex::RecursiveTryLock()
    {
        // We already have this mutex. Just refcount and return
        if (CThread::GetCurrentThreadID() == fHolder)
        {
            fHolderCount++;
            return true;
        }

    #ifdef __Win32__
        bool theErr = (bool)::TryEnterCriticalSection(&fMutex); // Return values of this function match our API
        if (!theErr)
            return theErr;
    #else
        int theErr = pthread_mutex_trylock(&fMutex);
        if (theErr != 0)
        {
            assert(theErr == EBUSY);
            return false;
        }
    #endif
        assert(fHolder == 0);
        fHolder = CThread::GetCurrentThreadID();
        fHolderCount++;
        assert(fHolderCount == 1);
        return true;
    }
    #endif //__PTHREADS_MUTEXES__ || __Win32__
}
