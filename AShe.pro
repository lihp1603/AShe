#-------------------------------------------------
#
# Project created by QtCreator 2019-03-15T11:22:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = AShe
TEMPLATE = lib

#添加包含目录
INCLUDEPATH += ./public/include
#add mysql
INCLUDEPATH +=  /usr/include/mysql
INCLUDEPATH +=  ./public/include/glog

#添加lib
LIBS += /work/github/AShe/AShe/public/libs/libxredis/lib/libxredis.a
LIBS += /work/github/AShe/AShe/public/libs/libhiredis/lib/libhiredis.a
LIBS += /work/github/AShe/AShe/public/libs/libglog/libglog.a

LIBS += -lmysqlclient
LIBS += -lpthread -ldl -lrt

DEFINES += ASHE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#添加
DEFINES += __PTHREADS_MUTEXES__
DEFINES += __PTHREADS__

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    public/OSCond.cpp \
    public/OSMutex.cpp \
    public/OSMutexRW.cpp \
    public/algorithm/cmiscalgorithm.cpp \
    public/algorithm/crandom.cpp \
    public/algorithm/DynamicPlanningAlgorithm.cpp \
    public/algorithm/md5.cpp \
    public/dbu/CMySql.cpp \
    public/dbu/CRedisClientWrap.cpp \
    public/loger/cerorr.cpp \
    public/loger/logtrace.cpp \
    public/prename/prename.c \
    public/time/clock.cpp \
    public/time/timeutils.cpp \
    public/OSThread.cpp \
    public/ctaskbase.cpp \
    public/Threadpool.cpp

HEADERS += \
    public/OSCond.h \
    public/OSMutex.h \
    public/OSMutexRW.h \
    public/algorithm/cmiscalgorithm.h \
    public/algorithm/crandom.h \
    public/algorithm/DynamicPlanningAlgorithm.h \
    public/algorithm/md5.h \
    public/dbu/CMySql.h \
    public/dbu/CRedisClientWrap.h \
    public/dbu/ErrorCode.h \
    public/dbu/MacroDefine.h \
    public/loger/cerorr.h \
    public/loger/logtrace.h \
    public/prename/prename.h \
    public/time/clock.h \
    public/time/timeutils.h \
    public/OSThread.h \
    public/ctaskbase.h \
    public/Threadpool.h \
    ashe.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=
