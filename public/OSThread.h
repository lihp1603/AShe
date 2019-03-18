#ifndef __OSThread_H_
#define __OSThread_H_

#include <string>
#include <atomic>

#ifdef __USE_JTHREAD_OSTHREAD_

#include "jthread/jthread.h"

using namespace jthread ;
#endif

using namespace std;

namespace PublicTool {

#ifdef __USE_JTHREAD_OSTHREAD_
    //定义一个系统线程类
    class OSThread:public JThread
    {
    public:
        OSThread();
        virtual ~OSThread();
        //线程切入，入口函数
        virtual void Entry()=0;

    #ifdef __Win32__
        static DWORD        GetCurrentThreadID() { return ::GetCurrentThreadId(); }
    #elif __PTHREADS__
        static  pthread_t   GetCurrentThreadID() { return ::pthread_self(); }
    #else
        static  cthread_t   GetCurrentThreadID() { return cthread_self(); }
    #endif
    private:
        //线程任务函数，私有了
        virtual void *Thread();
    };
#else
    //系统级别的线程类
    class OSThread{
    public:
        //定义一个线程回调函数的别名
        typedef void (ThreadFunc)(void* data);

        explicit OSThread(ThreadFunc* threadFunc,void* threadData,const std::string& nameArg="");
        ~ OSThread();

        //启动一个线程
        bool Start();
        //等待先从退出
        int32_t Join();
        //kill掉
        int32_t Kill();
        bool IsStarted(){return  m_bStarted;}
        const string& Name() const {return m_strName;}

//        pid_t Tid() const {return m_pid;}
        pthread_t GetCurrentThreadID(){return m_pthreadId;}
    private:
        //线程回调函数
        static void* RunInThread(void* param);
        //回调
        void RunThread();

        void SetDefaultName();

        bool m_bStarted;
        bool m_bJoined;
        std::string m_strName;
        pid_t m_pid;
        pthread_t m_pthreadId;

        ThreadFunc* m_fThreadFunc;
        void * m_pThreadData;
        //用一个原子变量来记录创建的线程数量
        static std::atomic<uint32_t> m_unNumCreated;
    };
#endif

    class CThread{
    public:
        CThread();
        virtual ~CThread();
        //线程切入，入口函数
        virtual void Entry()=0;

    #ifdef __Win32__
        static DWORD        GetCurrentThreadID() { return ::GetCurrentThreadId(); }
    #elif __PTHREADS__
        static  pthread_t   GetCurrentThreadID() {return ::pthread_self();}
    #else
        static  cthread_t   GetCurrentThreadID() { return cthread_self(); }
    #endif
    private:
        //线程任务函数，私有了
        static void ThreadCB(void* param);
        OSThread* m_pThread;
    };
}

#endif // OSThread_H
