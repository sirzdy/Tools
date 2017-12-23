#ifndef EXECUTETHREAD_H
#define EXECUTETHREAD_H

#include <QThread>

class ExecuteThread : public QThread
{
    Q_OBJECT
public:
    ExecuteThread(QString cmd);
    void run();

signals:
    void finish(QString cmd,int ret);
public slots:

private:
    QString m_sCmd;
};

#endif // EXECUTETHREAD_H
