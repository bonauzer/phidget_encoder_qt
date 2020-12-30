#include "threadTest.h"
#include "mainwindow.h"

threadTest::threadTest()
{
    bRun = false;
    nInterval = 0;
}

void threadTest::on() {
    bRun = true;
}

void threadTest::run()
{
    m_tm.start();

    // thread control
    bool bIdle = false;
    qint64 nStart = 0.0;
    qint64 nEnd = nStart;

    while (bRun)
    {
        nEnd = m_tm.nsecsElapsed();
        if (nEnd-nStart < 1000000)    bIdle = true;    // measuring 1 (ms) loop
        else                          bIdle = false;

        if(!bIdle)
        {
            if(pWnd->bRun)
            {
                pWnd->p_phidget->UpdateStates();
            }
            nInterval = (nEnd-nStart)/1000000;  // control loop interval (ms)
            nStart = nEnd;
        }
    }
    bRun = false;
}

void threadTest::stop()
{
    bRun = false;
}
