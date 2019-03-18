#include "ctaskbase.h"
#include <string>
#include <vector>
#include <assert.h>
#include <unistd.h>
#include "time/timeutils.h"

using namespace std;

namespace PublicTool {//任务管理模块


    /* ==========================================================================*
     * ================================CTaskBase=================================*
     * ==========================================================================*/

    CTaskBase::CTaskBase()
        :m_bExit(false)
        ,m_bRunning(false)
        ,m_llStartTimer(TimeMillis())
    {

    }

    CTaskBase::~CTaskBase()
    {
        WaitExitRun();
    }

    int64_t CTaskBase::GetTaskStartTimer()
    {
        return m_llStartTimer;
    }

    int64_t CTaskBase::GetTaskElapseTimer()
    {
        //获取耗时
        return TimeMillis()-m_llStartTimer;
    }

    void CTaskBase::WaitExitRun()
    {
        {
            if(IsRunning()){//如果任务还在运行中
                OSMutexLocker tmpLocker1(&m_mutexExit);
                m_bExit=true;//让其退出run任务函数
                while(m_bRunning)//等待完整退出
                    m_condExit.Wait(&m_mutexExit);//等待退出
            }
        }
    }

    void CTaskBase::SignalExitOK()
    {
        //通知退出
        OSMutexLocker tmpLocker1(&m_mutexExit);
        m_bExit=false;//
        m_bRunning=false;
        m_condExit.Signal();
    }

    bool CTaskBase::IsNeedExitRun()
    {
        OSMutexLocker tmpLocker1(&m_mutexExit);
        return m_bExit;
    }

    bool CTaskBase::SetExitRun()
    {
        OSMutexLocker tmpLocker1(&m_mutexExit);
        m_bExit=true;
    }

    bool CTaskBase::IsRunning()
    {
        OSMutexLocker tmpLocker1(&m_mutexExit);
        return m_bRunning;
    }

    void CTaskBase::StartRunning()
    {
         OSMutexLocker tmpLocker1(&m_mutexExit);
         m_bRunning=true;
         m_bExit=false;
    }


    void CTaskBase::do_task()
    {
        StartRunning();
        this->Run();
        //说明我们已经退出了
        SignalExitOK();
    }

}
