/*
 * 抽象任务模块功能
 * author:lihaiping1603@aliyun.com
 * date:2017-09-30 create
*/

#ifndef __CTASKBASE_H_
#define __CTASKBASE_H_

#include "OSCond.h"
#include "OSMutex.h"
#include <map>

using namespace std;

namespace PublicTool {//任务管理模块

    class CTaskBase{
    public:
         CTaskBase();
         virtual ~CTaskBase();

         //获取任务的开始时间
         int64_t GetTaskStartTimer();
         //获取这个任务流逝的时间：now-starttime
         int64_t GetTaskElapseTimer();
         //声明一个任务运行函数
         virtual void Run()=0;
         //做任务的函数
         void do_task();
    protected:
         //等待任务退出
         void WaitExitRun();
         //是否需要退出
         bool IsNeedExitRun();
         //设置退出
         bool SetExitRun();

    private:
         //表示run开始运行
         void StartRunning();
         //判断run是否在运行过程中
         bool IsRunning();
         //发送退出信号，表示run已经退出
         void SignalExitOK();


         //用于task退出
         OSMutex m_mutexExit;
         OSCond  m_condExit;
         bool m_bExit;
         bool m_bRunning;
         //定义一个task的开始时间
         int64_t m_llStartTimer;

    };

}

#endif // CTASKBASE_H
