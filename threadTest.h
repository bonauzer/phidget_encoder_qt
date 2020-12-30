#ifndef THREAD_H
#define THREAD_H


#include <QThread>
#include <QMutex>
#include <QElapsedTimer>
#include "main.h"

class threadTest : public QThread
{
    Q_OBJECT

public:
    threadTest();
//    ~threadTest();

    void    stop();
    void        on();
    double  nInterval;


protected:
    void run();

private:
    QElapsedTimer m_tm;
    QMutex mutex;
    volatile bool bRun;
};

#endif // THREAD_H
