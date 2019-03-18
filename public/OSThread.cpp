#include "OSThread.h"
#include <assert.h>
#include <atomic>

#include "./loger/logtrace.h"

#ifdef __USE_JTHREAD_OSTHREAD_
#include "jthread/jthread.h"

using namespace jthread ;
#endif

namespace PublicTool {

#ifdef __USE_JTHREAD_OSTHREAD_

    OSThread::OSThread()
        :JThread()
    {
        this->Start();
    }

    OSThread::~OSThread()
    {

    }

    void *OSThread::Thread()
    {
        //必须调用这个函数，告诉线程已经启动了
        this->ThreadStarted();
        this->Entry();
        return NULL;
    }
#else
    std::atomic<uint32_t> OSThread::m_unNumCreated;

    OSThread::OSThread(OSThread::ThreadFunc *threadFunc, void* threadData,const std::string &nameArg)
        :m_fThreadFunc(threadFunc)
        ,m_pThreadData(threadData)
        ,m_strName(nameArg)
        ,m_bStarted(false)
        ,m_bJoined(false)
        ,m_pid(0)
        ,m_pthreadId(0)
    {
        SetDefaultName();
    }

    OSThread::~OSThread()
    {
        if(m_bStarted&&!m_bJoined){
            pthread_detach(m_pthreadId);
        }
    }

    bool OSThread::Start()
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

        int32_t status= pthread_create(&m_pthreadId,&attr,RunInThread,this);
        pthread_attr_destroy(&attr);
        if(status!=0){
            return false;
        }
        return true;
    }

    int32_t OSThread::Join()
    {
        assert(m_bStarted);
        assert(!m_bJoined);
        m_bJoined=true;
        return pthread_join(this->m_pthreadId,NULL);
    }

    int32_t OSThread::Kill()
    {
        if(m_bStarted&&!m_bJoined){
            pthread_cancel(m_pthreadId);
        }
        return 0;
    }

    void *OSThread::RunInThread(void *param)
    {
         assert(param);
         OSThread*pTHd=(OSThread*)param;
         if(pTHd){
             pTHd->RunThread();
         }
         return param;
    }

    void OSThread::RunThread()
    {
        LogTraceD("the %s start...",this->Name());
        m_bStarted=true;
        if(this->m_fThreadFunc!=NULL){
            (*(this->m_fThreadFunc))(this->m_pThreadData);
        }
        LogTraceD("the %s end...",this->Name());
    }

    void OSThread::SetDefaultName()
    {
        assert(!m_bStarted);
        uint32_t num=m_unNumCreated++;
        if(m_strName.empty()){
            char buf[32]={0};
            snprintf(buf,sizeof(buf),"Thread%d",num);
            m_strName.assign(buf);
        }
    }

#endif
    CThread::CThread()
    {
        OSThread* pthread=new OSThread(CThread::ThreadCB,this);
        assert(pthread);
        if(!pthread->Start()){
            delete pthread;
            pthread=NULL;
        }
        m_pThread=pthread;
    }

    CThread::~CThread()
    {
        if(m_pThread){
            delete m_pThread;
            m_pThread=NULL;
        }
    }

    void CThread::ThreadCB(void *param)
    {
        assert(param);
        CThread* pcthread=(CThread*)param;
        if(pcthread){
            pcthread->Entry();
        }
    }
}
