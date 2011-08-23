/*pvServiceChannelRegister.cpp */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvDatabase.h>
#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

class PVServiceChannelRun : public Runnable {
public:
    PVServiceChannelRun();
    ~PVServiceChannelRun();
    virtual void run();
private:
    Event event;
    ServerContextImpl::shared_pointer ctx;
    Thread *thread;
};

PVServiceChannelRun::PVServiceChannelRun()
: 
  event(),
  ctx(ServerContextImpl::create()),
  thread(new Thread(String("pvServiceChannel"),lowerPriority,this))
{}

PVServiceChannelRun::~PVServiceChannelRun()
{
    ctx->shutdown();
    // we need thead.waitForCompletion()
    event.wait();
    epicsThreadSleep(1.0);
    delete thread;
}

void PVServiceChannelRun::run()
{
    ChannelProvider::shared_pointer channelProvider(
        PVServiceProvider::getPVServiceProvider());
    registerChannelProvider(channelProvider);
    ctx->setChannelProviderName(channelProvider->getProviderName());
    ctx->initialize(getChannelAccess());
    ctx->printInfo();
    ctx->run(0);
    ctx->destroy();
    event.signal();
}

static PVServiceChannelRun *myRun = 0;

static const iocshFuncDef startPVServiceChannelFuncDef = {
    "startPVServiceChannel", 0, 0};

static void startPVServiceChannelCallFunc(const iocshArgBuf *args)
{
    if(myRun!=0) {
        printf("PVServiceChannel already started\n");
        return;
    }
    myRun = new PVServiceChannelRun();
}

static const iocshFuncDef stopPVServiceChannelFuncDef = {
    "stopPVServiceChannel", 0, 0
};
static void stopPVServiceChannelCallFunc(const iocshArgBuf *args)
{
   printf("stopPVServiceChannel\n");
   if(myRun!=0) delete myRun;
   myRun = 0;
}

static void startPVServiceChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startPVServiceChannelFuncDef, startPVServiceChannelCallFunc);
    }
}

static void stopPVServiceChannelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopPVServiceChannelFuncDef, stopPVServiceChannelCallFunc);
    }
}

epicsExportRegistrar(startPVServiceChannelRegister);
epicsExportRegistrar(stopPVServiceChannelRegister);
