#include "executethread.h"
#include <QDebug>
ExecuteThread::ExecuteThread(QString cmd)
{
    m_sCmd=cmd;
}

void ExecuteThread::run()
{
    qDebug()<<m_sCmd;
    int ret=system(m_sCmd.toUtf8().data());
    emit finish(m_sCmd,ret);
}

