#ifndef __OSMUTEX_H_
#define __OSMUTEX_H_
#include "OSCond.h"
#include "OSThread.h"

namespace PublicTool {

    class OSMutex
    {
        public:

            OSMutex();
            ~OSMutex();

            inline void Lock();
            inline void Unlock();

            // Returns true on successful grab of the lock, false on failure
            inline bool TryLock();

        private:

    #ifdef __Win32__
            CRITICAL_SECTION fMutex;

            DWORD       fHolder;
            uint32_t      fHolderCount;

    #elif !__PTHREADS_MUTEXES__
            mymutex_t fMutex;
    #else
            pthread_mutex_t fMutex;
            // These two platforms don't implement pthreads recursive mutexes, so
            // we have to do it manually
            pthread_t   fHolder;
            uint32_t      fHolderCount;
    #endif

    #if __PTHREADS_MUTEXES__ || __Win32__
            void        RecursiveLock();
            void        RecursiveUnlock();
            bool        RecursiveTryLock();
    #endif
            friend class OSCond;
    };

    //自动锁
    class   OSMutexLocker
    {
        public:

            OSMutexLocker(OSMutex *inMutexP) : fMutex(inMutexP) { if (fMutex != NULL) fMutex->Lock(); }
            ~OSMutexLocker() {  if (fMutex != NULL) fMutex->Unlock(); }

            void Lock()         { if (fMutex != NULL) fMutex->Lock(); }
            void Unlock()       { if (fMutex != NULL) fMutex->Unlock(); }

        private:

            OSMutex*    fMutex;
    };

    inline void OSMutex::Lock()
    {
    #if __PTHREADS_MUTEXES__ || __Win32__
        this->RecursiveLock();
    #else
        mymutex_lock(fMutex);
    #endif //!__PTHREADS__
    }

    inline void OSMutex::Unlock()
    {
    #if __PTHREADS_MUTEXES__ || __Win32__
        this->RecursiveUnlock();
    #else
        mymutex_unlock(fMutex);
    #endif //!__PTHREADS__
    }

    inline bool OSMutex::TryLock()
    {
    #if __PTHREADS_MUTEXES__ || __Win32__
        return this->RecursiveTryLock();
    #else
        return (bool)mymutex_try_lock(fMutex);
    #endif //!__PTHREADS__
    }


}

#endif // OSMutex_H
