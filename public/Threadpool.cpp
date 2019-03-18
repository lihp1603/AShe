#include "Threadpool.h"
#include "stdint.h"
#include "ctaskbase.h"
#include <assert.h>
#include <string>

namespace PublicTool {

ThreadPool::ThreadPool(const string &nameArg)
    :m_strName(nameArg)
    ,m_bRunning(false)
    ,m_unMaxQueueSize(1000)
{
    m_dqTasks.clear();
}

ThreadPool::~ThreadPool()
{

}

bool ThreadPool::Start(int32_t numThreads)
{
    assert(m_vecThreads.empty());
//    std::string poolName=m_strName;
    m_bRunning=true;

    for(int i=0;i<numThreads;i++){
        OSThread* pthread=new OSThread(ThreadPool::Process,this,m_strName);
        assert(pthread);
        if(pthread->Start()){
            this->m_vecThreads.push_back(pthread);
        }else{
            delete pthread;
            pthread=NULL;
            this->Stop();
            return false;
        }
    }

    return true;
}

void ThreadPool::Stop()
{
    OSMutexLocker lock(&m_dqMutex);
    m_bRunning=false;
    for(int i=0;i<m_vecThreads.size();i++){
        OSThread* pthread =m_vecThreads.at(i);
        if(pthread){
            delete pthread;
            pthread=NULL;
        }
    }
    m_vecThreads.clear();
}

bool ThreadPool::Run(CTaskBase *task)
{
   assert(task);
   if(m_vecThreads.empty()){
        if(task){
            task->do_task();
        }
   }else{
        OSMutexLocker lock(&m_dqMutex);
        while(isFull()&&m_bRunning){//如果满了，就等任务消耗消息
            m_dqNotFull.Wait(&m_dqMutex);
        }
        m_dqTasks.push_back(task);
        m_dqNotEmpty.Signal();//通知
   }
   return true;
}

CTaskBase *ThreadPool::GetTask()
{
    CTaskBase *task=NULL;
    OSMutexLocker lock(&m_dqMutex);
    while(m_dqTasks.empty()&&m_bRunning){//如果队列为空就等待
        m_dqNotEmpty.Wait(&m_dqMutex);
    }
    if(!m_dqTasks.empty()){
        task=m_dqTasks.front();
        m_dqTasks.pop_front();
        if(m_unMaxQueueSize>0){
            m_dqNotFull.Signal();
        }
    }
    return task;
}

void ThreadPool::ProcessEx()
{
    while(isRunning()){
        CTaskBase* task=GetTask();
        if(task){
            task->do_task();
        }
    }
}

void ThreadPool::Process(void *param)
{
    ThreadPool* ppool=(ThreadPool*)param;
    if(ppool){
        ppool->ProcessEx();
    }
}



}
