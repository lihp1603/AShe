#ifndef __CTASKTHREADPOOL_H_
#define __CTASKTHREADPOOL_H_

#include "stdint.h"
#include "ctaskbase.h"
#include "OSCond.h"
#include "OSMutex.h"
#include <string>
#include <vector>
#include <deque>

using namespace std;

namespace PublicTool {


    class ThreadPool {
    public:
        explicit ThreadPool(const string &nameArg= string("ThreadPool"));
        ~ThreadPool();
        //启动多个底层线程
        bool Start(int32_t numThreads);
        //停止线程池中的线程
        void Stop();
        //运行一个任务
        bool Run(CTaskBase* task);

        void SetMaxQueueSize(uint32_t maxSize){
            m_unMaxQueueSize=maxSize;
        }

    private:
        bool isFull(){
            return m_dqTasks.size()>=m_unMaxQueueSize?true:false;
        }
        bool isRunning(){
              OSMutexLocker lock(&m_dqMutex);
              return m_bRunning;
        }

        //获取一个运行的任务
        CTaskBase* GetTask();

        void ProcessEx();
        static void Process(void * param);

        std::string m_strName;
        std::vector<OSThread*> m_vecThreads;//存储线程池中的线程
        //用于多线程直接获取任务队列之间的Lock
        OSMutex m_dqMutex;
        OSCond  m_dqNotEmpty;//
        OSCond  m_dqNotFull;
        std::deque<CTaskBase*> m_dqTasks;//任务队列
        bool m_bRunning;
        uint32_t m_unMaxQueueSize;
    };
}

#endif // CTASKTHREADPOOL_H
