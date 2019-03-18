/********************************************************
 * 主要是封装一些常用的公共功能函数，方便项目中使用，
 * 例如线程，线程池，锁，redis,mysql，日志，进程改名等工具
 * author:lihaiping1603@aliyun.com
 * date:2019-03-18
*********************************************************/
#ifndef __ASHE_H__
#define __ASHE_H__


#if defined(ASHE_LIBRARY)
#  define ASHESHARED_EXPORT
#else
#  define ASHESHARED_EXPORT
#endif


#include "public/loger/logtrace.h"
#include "public/OSMutex.h"
#include "public/OSCond.h"
#include "public/OSThread.h"
#include "public/Threadpool.h"
#include "public/ctaskbase.h"
#include "public/prename/prename.h"
#include "public/time/timeutils.h"
#include "public/dbu/CRedisClientWrap.h"
#include "public/dbu/CMySql.h"



#endif // ASHE_GLOBAL_H
